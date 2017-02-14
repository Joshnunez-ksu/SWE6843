#include "TestStates.h"
#include <iostream>
#include <time.h>

//returns the current number of milliseconds
long getTick()
{
      struct timespec ts;
      timespec_get(&ts, TIME_UTC);
      
      return (ts.tv_sec * 1000) + (ts.tv_nsec/1000000);
}
      
State* StateInitial::processInput(void* data)
{
      std::cout << "Start StateInitial\n";
      
      GameData* gameData = (GameData*) data;
      
      gameData->startTick = 0;
      gameData->playerOneTick = 0;
      gameData->playerTwoTick = 0;

      GPIOSystem* gpio = (GPIOSystem*) this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO);
      
      // initialize GPIO 2
      GPIOPin* pin = gpio->getPin(2);
      pin->setDirection(OUT);
      pin->setValue(LOW);
      
      // initialize GPIO 21
      pin = gpio->getPin(21);
      pin->setDirection(IN);
      
      std::cout << "End StateInitial\n";
      
      ((GameData*) data)->startTick = getTick();
      
      std::cout << "Starting StateBeforeGame\n";
      return this->stateManager->getState("StateBeforeGame");
}

StateBeforeGame::StateBeforeGame(StateManager* sm, PeripheralFactory* pf) : State(sm, pf)
{
      this->LED = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(2);
      this->theButton = new Button(((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(21));
      this->pressCount = 0;
}

StateBeforeGame::~StateBeforeGame()
{
      delete this->theButton;
}

State* StateBeforeGame::processInput(void* data)
{
      State* returnState = this;
      
      if (this->theButton->pressed())
      {
            this->pressCount++;
            
            //toggle the LED
            if (this->LED->getValue() == HIGH)
            {
                  this->LED->setValue(LOW);
            }
            else
            {
                  this->LED->setValue(HIGH);
            }
      }

      // after 10 button presses change to the state
      if (this->pressCount >= 10)
      {
            std::cout << "End StateBeforeGame\n";
            std::cout << "Starting StateAfterGame\n";
            returnState = this->stateManager->getState("StateAfterGame");
      }
      
      return returnState;
}

State* StateAfterGame::processInput(void* data)
{
      GameData* gameData = (GameData*) data;
      std::cout << "Start Tick = " << gameData->startTick << "\n";
      std::cout << "Tick Diff = " << getTick() - gameData->startTick << "\n";
      
      timespec ts;
      ts.tv_sec = 3;
      ts.tv_nsec = 0;
      nanosleep(&ts, &ts);
      
      std::cout << "End StateAfterGame\n";
      
      return (State*) 0;
}
