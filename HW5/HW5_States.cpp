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

void StateInitial::setup()
{
      GPIOSystem* gpio = (GPIOSystem*) this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO);
      
      GPIOPin* pin = gpio->getPin(pin0);
      //pin->setPull(x);
      pin->setDirection(OUT);
      pin->setValue(LOW);
      
      pin = gpio->getPin(pin1);
      //pin->setPull(x);
      pin->setDirection(OUT);
      pin->setValue(LOW);
      
      //Setup the rest of the pins
}

State* StateInitial::process(void* data)
{
      State* returnState = this;
      returnState = this->stateManager->getState("StateBeforeGame");
      return returnState;
}

void StateBeforeGame::setup()
{
      //Initialize and set game data to 0
      GameData* gameData = (GameData*) data;
      gameData->startTick = 0;
      gameData->playerOneTick = 0;
      gameData->playerTwoTick = 0;
      
      //Initialize input
      this->startButton = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(x);
      
      //Initialize output
      playerOneDisplay1 = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(x);
      playerOneDisplay2 = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(x);
      //playerTwoDisplay = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(x);
      
      //set playerOneDisplay to 10 seconds
      //set playerTwoDisplay to 10 seconds
}

State* StateBeforeGame::process(void* data)
{
      State* returnState = this;
      if(this->startButton->pressed()) returnState = this->stateManager->getState("StatePreInGame");
      
      return returnState;
}

void StatePreInGame::setup()
{
      //Initialize input
      //No input
      
      //Initialize output
      LED1 = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(x);
      LED2 = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(x);
      LED3 = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(x);
      LED4 = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(x);
      LED5 = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(x);
      LED6 = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(x);
      
      buzzer = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(x);
}

State* StatePreInGame::process(void* data)
{
      State* returnState = this;
      
      //Countdown buzzer
      //pitch, duration
      /*
       * LED1.write(HIGH);
       * buzzer.makeSound(250, .50);
       * sleep(1.0);
       * LED1.write(LOW);
       *
       * LED2.write(HIGH);
       * buzzer.makeSound(250, .50);
       * sleep(.25);
       * LED2.write(LOW);
       *
       * LED3.write(HIGH);
       * buzzer.makeSound(250, .50);
       * sleep(.25);
       * LED3.write(LOW);
       *
       * LED4.write(HIGH);
       * buzzer.makeSound(250, .50);
       * sleep(.25);
       * LED4.write(LOW);
       *
       * LED5.write(HIGH);
       * buzzer.makeSound(250, .50);
       * sleep(.25);
       * LED5.write(LOW);
       *
       * LED6.write(HIGH);
       * buzzer.makeSound(250, .50);
       * sleep(.25);
       * LED6.write(LOW);
       */
      
      returnState = this->stateManager->getState("StateWaitForOne");
      
      return returnState;
}

void StateWaitForOne::setup()
{
      //Initialize input
      theButton = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(x);
      
      //Initialize output
      playerOneDisplay1 = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(x);
      playerOneDisplay2 = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(x);
      countdown = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(x);
      countup = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(x);
}

State* StateWaitForOne::process(void* data)
{
      State* returnState = this;
      
      //Start timer countdown
      /*
       * countdown.write(HIGH);
       * if(timer is counting up)
       * {
       *    countdown.write(LOW);
       *    countup.write(HIGH);
       * }
       * else if(timer has counted up and reached zero) countup.write(LOW);
       */
      //write time to display
      ((GameData*) data)->start();
      
      if(this->theButton->pressed())
      {
            //Stop countdown
            //countdown.write(LOW);
            //countdown.write(LOW);
            //Log current time as player's time
            returnState = this->stateManager->getState("StatePostGame");
      }
      
      return returnState;
}

void StatePostGame::setup()
{
      //Initialize input
      startOver = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(x);
      
      //Initialize output
      playerOneDisplay1 = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(x);
      playerOneDisplay2 = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(x);
}

State* StatePostGame::process(void* data)
{
      State* returnState = this;
      if(this->startOver->pressed()) returnState = this->stateManager->getState("StateBeforeGame");
      return returnState;
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
