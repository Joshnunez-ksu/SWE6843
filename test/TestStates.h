#include "../common/State.h"

struct GameData
{
      long startTick;
      long playerOneTick;
      long playerTwoTick;
};

class StateInitial : public State
{
public:
      StateInitial(StateManager* sm, PeripheralFactory* pf) : State(sm, pf){};
      State* processInput(void* data);
      
};

class StateBeforeGame : public State
{
public:
      StateBeforeGame(StateManager* sm, PeripheralFactory* pf);
      ~StateBeforeGame();
      State* processInput(void* data);
      
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
      State* processInput(void* data);
};