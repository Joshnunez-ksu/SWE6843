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
            StateInitial(StateManager* sm, PeripheralFactory* pf) : State(sm, pf) {};
            void setup();
            State* process(void* data);
};

class StateBeforeGame : public State
{
      public:
            StateBeforeGame(StateManager* sm, PeripheralFactory* pf) : State(sm, pf) {};
            ~StateBeforeGame();
            void setup();
            State* process(void* data);

       private:
            Button* startButton;
            SingleDigitDisplay* playerOneDisplay, playerTwoDisplay;
};

class StatePreInGame : public State
{
      public:
            StatePreInGame(StateManager* sm, PeripheralFactory* pf) : State(sm, pf) {};
            void setup();
            State* process(void* data);
};

class StateWaitForTwo : public State
{
      public:
            StateWaitForTwo(StateManager* sm, PeripheralFactory* pf) : State(sm, pf) {};
            void setup();
            State* process(void* data);
};

class StateWaitForOne : public State
{
      public:
            StateWaitForOne(StateManager* sm, PeripheralFactory* pf) : State(sm, pf) {};
            void setup();
            State* process(void* data);
};

class StatePostGame : public State
{
      public:
            StatePostGame(StateManager* sm, PeripheralFactory* pf) : State(sm, pf) {};
            void setup();
            State* process(void* data);
};
