#pragma once

#define LEVEL_ONE_BETWEEN 250
#define LEVEL_ONE_AFTER 250

#define LEVEL_TWO_BETWEEN 500
#define LEVEL_TWO_AFTER 250

#define LEVEL_THREE_BETWEEN 750
#define LEVEL_THREE_AFTER 250

enum BuzzLevel : ushort
{
    one,
    two,
    three
};

class HapticMotor
{
    private:
        int between_buzz;
        int after_buzz;
        BuzzLevel buzzLevel;
        void LevelOneBuzz();
        void LevelTwoBuzz();
        void LevelThreeBuzz();
        void ExecuteBuzz();

    public:
        HapticMotor();
        void Setup();
        void SetBuzzLevel(int n);
        void SetBuzz(int between_buzz, int after_buzz);
        void BuzzMotor(int n);
};