#include "Pins.h"
#include "Blink.h"

void Blink::Setup()
{
    pinMode(LED_PIN, OUTPUT);
}

Blink::Blink() { }

void Blink::Run()
{
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));   // turn the LED on (HIGH is the voltage level)
}