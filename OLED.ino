#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Pins.h"
#include "OLED.h"
#define I2C_SCL 22
#define I2C_SDA 23
OLED::OLED()
{
    Adafruit_SSD1306 d(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
    display = d;
}

void OLED::Setup()
{
    // Set up SCL and SDA pins
    // We're using ESP32 Dev Module; it has no idea where SCL and SDA are
    Wire.begin(I2C_SDA, I2C_SCL);
    display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
}

void OLED::AddMessage(String header, String message)
{
    // Clear Display
  display.clearDisplay();

  // Set text color
  display.setTextColor(WHITE);

  // Set font size
  display.setTextSize(1);

  // Set cursor to top corner
  display.setCursor(0, 0);

  // Print message
  display.println(header);

  display.setTextSize(1);
  display.setCursor(0, 20);
  display.println(message);
}

void OLED::UpdateDisplay()
{
    display.display();
}