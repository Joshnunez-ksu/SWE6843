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
            
      private:
            GPIOPin* LED1, LED2, LED3, LED4, LED5, LED6;
            Buzzer* buzzer;
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
      
      private:
            Button* theButton;
            SingleDigitDisplay* playerOneDisplay;
            GPIOPin* countdown, countup;
};

class StatePostGame : public State
{
      public:
            StatePostGame(StateManager* sm, PeripheralFactory* pf) : State(sm, pf) {};
            void setup();
            State* process(void* data);
            
      private:
            Button* startOver;
            SingleDigitDisplay playerOneDisplay;
};
