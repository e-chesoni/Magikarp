#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Pins.h"
#include "Blink.h"
#include "Button.h"
#include "OLED.h"
#include "HapticMotor.h"


// Timer Settings
#define TIMER_NO 0
#define TIMER_PRESCALER 80
#define TIMER_INTERVAL 1000000

Blink blinker;
OLED oled;
HapticMotor buzzer;

/********************************************/
/*           Interrupt Variables            */
/********************************************/
// Timer
hw_timer_t *timer = NULL;
volatile SemaphoreHandle_t timerSemaphore;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
volatile uint32_t isrCounter = 0;
volatile uint32_t lastIsrAt = 0;
volatile uint32_t interrupt_counter = 0;

// Button
unsigned long button_time = 0;  
unsigned long last_button_time = 0;
unsigned long debounce_interval = 250;
Button timer_reset_button = {BUTTON_PIN, 0, false};

/********************************************/
/*           OLED Variables            */
/********************************************/

// OLED Variables
bool NOTIFY = true; // start notify is true; change after first loop


/********************************************/
/*           Interrupt Functions            */
/********************************************/
/// Calls to IRAM for interrupts (Forces code to run on IRAM instead of Flash)

/// @brief Interrupt Service Routine for Button Interrupt
void IRAM_ATTR Button_ISR() {
	// Keep track of last button press so we can debounce button
	// NOTE: Still some issues if you hold the button for a couple seconds
	button_time = millis();
	if (button_time - last_button_time > debounce_interval)
	{
        timer_reset_button.numberKeyPresses++;
        timer_reset_button.pressed = true;
       last_button_time = button_time;
	}
}

void IRAM_ATTR OnTimer(){
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);
  isrCounter++;
  lastIsrAt = millis();
  portEXIT_CRITICAL_ISR(&timerMux);
  // Give a semaphore that we can check in the loop
  xSemaphoreGiveFromISR(timerSemaphore, NULL);
  // It is safe to use digitalRead/Write here if you want to toggle an output
  //digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  blinker.Run();
  interrupt_counter++;
}


/********************************************/
/*              Setup Functions             */
/********************************************/
void SetupTimer()
{
  // Create semaphore to inform us when the timer has fired
  timerSemaphore = xSemaphoreCreateBinary();

  // Use timer 0, prescale 80, and count up to (true)
  timer = timerBegin(TIMER_NO, TIMER_PRESCALER, true);

  // Attach timer to ISR defined above
  timerAttachInterrupt(timer, &OnTimer, true);

  // Generate interrupt every 1000000 micro seconds (1 sec)
  // true = repeat the alarm
  timerAlarmWrite(timer, TIMER_INTERVAL, true);

  // Enable timer interrupt
  timerAlarmEnable(timer);
}

void SetupButtonInterrupt()
{
	pinMode(timer_reset_button.PIN, INPUT_PULLUP);
	attachInterrupt(timer_reset_button.PIN, Button_ISR, FALLING);
}

void SetDebounceInterval(int interval)
{
    debounce_interval = interval;
}

void CheckButton()
{
  if (timer_reset_button.pressed) {
		Serial.println("Button has been pressed. Resetting timer...");
    timerWrite(timer, 0);
    interrupt_counter = 0;
		timer_reset_button.pressed = false;
    NOTIFY = true;
	} else {
    // If timer has fired
    if (xSemaphoreTake(timerSemaphore, 0) == pdTRUE)
    {
      // Print interrupt counter
      Serial.print("onTimer no. ");
      Serial.println(interrupt_counter);
    }
  }
}

/********************************************/
/*           Initialize Components          */
/********************************************/
void InitializeComponents()
{
  blinker.Setup();
  oled.Setup();
  buzzer.Setup(); // Sets buzz level to 1 by default

  SetupTimer();
  SetupButtonInterrupt();
  SetDebounceInterval(250);
}

void setup() {
  Serial.begin(115200);
  InitializeComponents();
}

void loop() {
  CheckButton();

  if (interrupt_counter == 10)
  {
    oled.AddMessage("Hydro Check: ", "Are you drinking enough water?");
    oled.UpdateDisplay();
    buzzer.SetBuzzLevel(1);
    buzzer.BuzzMotor(2);
    NOTIFY = false;
  }
  
  if (interrupt_counter == 20)
  {
    oled.AddMessage("Hydro Check: ", "Probs time to drink some water");
    oled.UpdateDisplay();
    buzzer.SetBuzzLevel(2);
    buzzer.BuzzMotor(2);
    NOTIFY = false;
  }

  if (interrupt_counter == 30)
  {
    oled.AddMessage("Hydro Check: ", "You're on the verge of dehydration...");
    oled.UpdateDisplay();
    buzzer.SetBuzzLevel(3);
    buzzer.BuzzMotor(2);
    NOTIFY = false;
  }

  // Display OLED Message
  if (NOTIFY)
  {
    oled.AddMessage("Header 1", "First message");
    oled.UpdateDisplay();
    NOTIFY = false;
  }
}
