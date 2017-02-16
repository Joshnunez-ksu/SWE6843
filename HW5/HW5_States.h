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
};

class StatePreInGame : public State
{

};

class StateInGame : public State
{

};

class StatePostGame : public State
{

};
