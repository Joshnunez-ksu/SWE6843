#include "../common/State.h"

struct GameData
{
      long startTick;
      long playerOneTick;
      long playerTwoTick;
      SingleDigitDisplay* displayOne;
      SingleDigitDisplay* displayTwo;
      SingleDigitDisplay* displayThree;
      SingleDigitDisplay* displayFour;
      
      GPIOPin* LED1;
      GPIOPin* LED2;
      GPIOPin* LED3;
      
      Button* buttonOne;
      Button* buttonTwo;
      Button* buttonThree;
      
      SoftBuzzer* buzzer;
};

class StateInitial : public State
{
public:
      StateInitial(StateManager* sm, PeripheralFactory* pf) : State(sm, pf){};
      State* process(void* data);
      void setup(void* data);
      
};

class StateBeforeGame : public State
{
public:
      StateBeforeGame(StateManager* sm, PeripheralFactory* pf);
      ~StateBeforeGame();
      State* process(void* data);
      void setup(void* data);
      
 private:
      Button* theButton;
      GPIOPin* LED;
      int pressCount;
      SingleDigitDisplay* theDisplay;
      SingleDigitDisplay* secDisplay;
};

class StateAfterGame : public State
{
public:
      StateAfterGame(StateManager* sm, PeripheralFactory* pf) : State(sm, pf){};
      State* process(void* data);
      void setup(void* data);
};
