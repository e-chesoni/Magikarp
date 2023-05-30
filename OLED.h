#pragma once

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Pins.h"

// OLED Defines
#define OLED_ADDR   0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4 // Set this value to any data pin (even though you don't use it)

class OLED
{
    private:
        Adafruit_SSD1306 display;

    public:
        OLED();
        void Setup();
        void AddMessage(String header, String message);
        void UpdateDisplay();
};