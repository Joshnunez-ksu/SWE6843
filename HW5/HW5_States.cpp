#include "HW5_States.h"
#include <iostream>
#include <time.h>

const int pin0 = 2;
const int pin1 = 3;
const int pin2 = 19;
const int pin3 = 26;
const int pin4 = 21;

//returns the current number of milliseconds
long getTick()
{
      struct timespec ts;
      timespec_get(&ts, TIME_UTC);
      
      return (ts.tv_sec * 1000) + (ts.tv_nsec/1000000);
}

State::State(StateManager* sm, PeripheralFactory* pf)
{
      this->stateManager = sm;
      this->peripheralFactory = pf;
}

State* StateInitial::processData(void* data)
{
      std::cout << "Start StateInitial!\n";
      
      GameData* gameDate = (GameData*) data;
      gameData->startTick = 0;
      gameData->playerOneTick = 0;
      gameData->playerTwoTick = 0;
      
      GPIOSystem* gpio = (GPIOSystem*) this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO);
      
      GPIOPin* pin = gpio->getPin(pin0);
      //pin->setPull(x);
      pin->setDirection(OUT);
      pin->setValue(LOW);
      
      pin = gpio->getPin(pin1);
      //pin->setPull(x);
      pin->setDirection(OUT);
      pin->setValue(LOW);
      
      std::cout << "End StateInitial!\n";
      return this->stateManager->getState("StateBeforeGame");
}

StateManager::StateManager()
{
      
}

void StateManager::registerState(const char* stateName, State* state)
{
      stateMap[stateName] = state;
}

State* StateManager::getState(const char* stateName)
{
      if (stateMap.find(stateName) == stateMap.end())
      {
            return (State*) 0;
      }
      else
      {
            return stateMap[stateName];
      }
}
