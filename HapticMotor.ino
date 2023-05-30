#include "Pins.h"
#include "HapticMotor.h"

HapticMotor::HapticMotor() 
{
    SetBuzzLevel(1);
}

void HapticMotor::Setup()
{
    pinMode(HAPTIC_INPUT_PIN, OUTPUT);
}

void HapticMotor::SetBuzzLevel(int n)
{
    switch (n)
        {
            case 1:
                buzzLevel = one;
                break;
            case 2:
                buzzLevel = two;
                break;
            case 3:
                buzzLevel = three;
                break;
            default:
                break;
        }
}

void HapticMotor::SetBuzz(int b, int a)
{
    between_buzz = b;
    after_buzz = a;
}

// Helpers
void HapticMotor::ExecuteBuzz()
{
    digitalWrite(HAPTIC_INPUT_PIN, HIGH);
	delay(between_buzz);
    digitalWrite(HAPTIC_INPUT_PIN, LOW);
    delay(after_buzz);
}

void HapticMotor::LevelOneBuzz()
{
    SetBuzz(LEVEL_ONE_BETWEEN, LEVEL_ONE_AFTER);
    ExecuteBuzz();
}

void HapticMotor::LevelTwoBuzz()
{
    SetBuzz(LEVEL_TWO_BETWEEN, LEVEL_TWO_AFTER);
    ExecuteBuzz();
}

void HapticMotor::LevelThreeBuzz()
{
    SetBuzz(LEVEL_THREE_BETWEEN, LEVEL_THREE_AFTER);
    ExecuteBuzz();
}

void HapticMotor::BuzzMotor(int n)
{
    int ctr = n;

    while(ctr > 0)
    {
        Serial.println("Buzzing motor!"); 
        switch (buzzLevel)
        {
            case one:
                LevelOneBuzz();
                break;
            case two:
                LevelTwoBuzz();
                break;
            case three:
                LevelThreeBuzz();
                break;
            default:
                break;
        }
	    
        ctr--;
    }
}