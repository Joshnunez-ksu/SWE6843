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
            void setup(void* data);
            State* process(void* data);
};

class StateBeforeGame : public State
{
      public:
            StateBeforeGame(StateManager* sm, PeripheralFactory* pf) : State(sm, pf) {};
            ~StateBeforeGame();
            void setup(void* data);
            State* process(void* data);

       private:
            Button* startButton;
            SingleDigitDisplay* playerOneDisplay1;
            SingleDigitDisplay* playerOneDisplay2;
};

class StatePreInGame : public State
{
      public:
            StatePreInGame(StateManager* sm, PeripheralFactory* pf) : State(sm, pf) {};
            void setup(void* data);
            State* process(void* data);
            
      private:
            GPIOPin* LED1;
            GPIOPin* LED2;
            GPIOPin*  LED3;
            GPIOPin*  LED4;
            GPIOPin*  LED5;
            GPIOPin*  LED6;
            Buzzer* buzzer;
};

class StateWaitForTwo : public State
{
      public:
            StateWaitForTwo(StateManager* sm, PeripheralFactory* pf) : State(sm, pf) {};
            void setup(void* data);
            State* process(void* data);
};

class StateWaitForOne : public State
{
      public:
            StateWaitForOne(StateManager* sm, PeripheralFactory* pf) : State(sm, pf) {};
            void setup(void* data);
            State* process(void* data);
      
      private:
            Button* theButton;
            SingleDigitDisplay* playerOneDisplay1;
            SingleDigitDisplay* playerOneDisplay2;
            GPIOPin* countdown;
            GPIOPin* countup;
};

class StatePostGame : public State
{
      public:
            StatePostGame(StateManager* sm, PeripheralFactory* pf) : State(sm, pf) {};
            void setup(void* data);
            State* process(void* data);
            
      private:
            Button* startOver;
            SingleDigitDisplay* playerOneDisplay1;
            SingleDigitDisplay* playerOneDisplay2;
};
