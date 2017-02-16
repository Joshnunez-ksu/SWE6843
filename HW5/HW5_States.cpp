#include "HW5_States.h"
#include <iostream>
#include <time.h>

/* PlayerOneDisplay1 pins */
const int pin0 = 2;
const int pin1 = 3;
const int pin2 = 4; //GPIO 4
const int pin3 = 5; //GPIO 5
const int pin4 = 13; //GPIO 13. Decimal.

/* PlayerOneDisplay2 pins */
const int pin5 = 17; //GPIO 17
const int pin6 = 27; //GPIO 27
const int pin7 = 22; //GPIO 22
const int pin8 = 6; //GPIO 6
const int pin9 = 19; //GPIO 19. Decimal.

/* Buzzer pin */
//const int pinBuzzer = ?;

/* PlayerOneButton */
const int pinPlayerOneButton = 21;

/* Start button */
const int pinStartButton = 20;

/* Countup & Countdown pins */
const int pinCountup = 23; //GPIO 23
const int pinCountdown = 24; //GPIO 24

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
      
      GPIOPin* pin;
      
      /* START PlayerOneDisplay1 */
      pin = gpio->getPin(pin0);
      //pin->setPull(x);
      pin->setValue(LOW);
      
      pin = gpio->getPin(pin1);
      //pin->setPull(x);
      pin->setValue(LOW);
      
      pin = gpio->getPin(pin2);
      //pin->setPull(x);
      pin->setValue(LOW);
      
      pin = gpio->getPin(pin3);
      //pin->setPull(x);
      pin->setValue(LOW);
      
      pin = gpio->getPin(pin4);
      //pin->setPull(x);
      pin->setValue(LOW);
      /* END PlayerOneDisplay1 */
      
      /* START PlayerOneDisplay2 */
      pin = gpio->getPin(pin0); //First pin: GPIO 2
      //pin->setPull(x);
      pin->setValue(LOW);
      
      pin = gpio->getPin(pin1); //Second pin: GPIO 3
      //pin->setPull(x);
      pin->setValue(LOW);
      
      pin = gpio->getPin(pin2); //Second pin: GPIO 4
      //pin->setPull(x);
      pin->setValue(LOW);
      
      pin = gpio->getPin(pin3); //Second pin: GPIO 15
      //pin->setPull(x);
      pin->setValue(LOW);
      /* END PlayerOneDisplay2 */
      
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
      this->startButton = new Button((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO)->getPin(pinStartButton));
      
      //Initialize output
      this->playerOneDisplay1 = new SingleDigitDisplay(
                                                       (GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO)->getPin(pin0),
                                                       (GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO)->getPin(pin1),
                                                       (GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO)->getPin(pin2),
                                                       (GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO)->getPin(pin3),
                                                       (GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO)->getPin(pin4)
                                                      );
      
      this->playerOneDisplay2 = new SingleDigitDisplay(
                                                       (GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO)->getPin(pin5),
                                                       (GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO)->getPin(pin6),
                                                       (GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO)->getPin(pin7),
                                                       (GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO)->getPin(pin8),
                                                       (GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO)->getPin(pin9)
                                                      );
      
      //set playerOneDisplay to 10 seconds
      this->playerOneDisplay1->setDisplay(1);
      this->playerOneDisplay2->setDisplay(0);
}

State* StateBeforeGame::process(void* data)
{
      State* returnState = this;
      if(this->startButton->pressed())
      {
            returnState = this->stateManager->getState("StatePreInGame");
      }
      
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
