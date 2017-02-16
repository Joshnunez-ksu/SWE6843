#include "HW5_States.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>

//returns the current number of milliseconds
long getTick()
{
      struct timespec ts;
      timespec_get(&ts, TIME_UTC);
      
      return (ts.tv_sec * 1000) + (ts.tv_nsec/1000000);
}

void StateInitial::setup(void* data)
{
      std::cout << "StateInitial\n";
      GPIOSystem* gpio = (GPIOSystem*) this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO);
      
      GPIOPin* pin;
      
      /* START PlayerOneDisplay1 */
      pin = gpio->getPin(2);
      //pin->setPull(x);
      pin->setValue(LOW);
      
      pin = gpio->getPin(3);
      //pin->setPull(x);
      pin->setValue(LOW);
      
      pin = gpio->getPin(4);
      //pin->setPull(x);
      pin->setValue(LOW);
      
      pin = gpio->getPin(5);
      //pin->setPull(x);
      pin->setValue(LOW);
      
      pin = gpio->getPin(13);
      //pin->setPull(x);
      pin->setValue(LOW);
      /* END PlayerOneDisplay1 */
      
      /* START PlayerOneDisplay2 */
      pin = gpio->getPin(17);
      //pin->setPull(x);
      pin->setValue(LOW);
      
      pin = gpio->getPin(27);
      //pin->setPull(x);
      pin->setValue(LOW);
      
      pin = gpio->getPin(22);
      //pin->setPull(x);
      pin->setValue(LOW);
      
      pin = gpio->getPin(6);
      //pin->setPull(x);
      pin->setValue(LOW);
      
      pin = gpio->getPin(19);
      //pin->setPull(x);
      pin->setValue(LOW);
      /* END PlayerOneDisplay2 */
      
      //Setup the rest of the pins
}

State* StateInitial::process(void* data)
{
      //Initialize output
      SingleDigitDisplay disp1(
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(2),
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(3),
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(4),
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(5),
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(13)
                                                      );
      
      SingleDigitDisplay disp2(
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(17),
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(27),
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(22),
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(6),
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(19)
                                                      );
      
      //set playerOneDisplay to 10 seconds
      disp1.setDisplay(1);
      disp2.setDisplay(0);
      
      State* returnState = this;
      returnState = this->stateManager->getState("StateBeforeGame");
      return returnState;
}

void StateBeforeGame::setup(void* data)
{     
      std::cout << "StateBeforeGame\n"; 
      //Initialize input
      this->startButton = new Button(((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(20));
      
}

State* StateBeforeGame::process(void* data)
{
      State* returnState = this;
      if(this->startButton->pressed())
      {
            std::cout << "BeforeGAme button press\n";
            returnState = this->stateManager->getState("StatePreInGame");
      }
      
      return returnState;
}

void StatePreInGame::setup(void* data)
{
std::cout << "StatePreInGame\n";
      //Initialize and set game data to 0
      GameData* gameData = (GameData*) data;
      gameData->startTick = 0;
      gameData->playerOneTick = 0;
      gameData->playerTwoTick = 0;

      this->countUpLED = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(23);
      this->countUpLED->setValue(LOW);
      
      this->countDownLED = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(24);
      this->countDownLED->setValue(HIGH);
      
      SingleDigitDisplay disp1(
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(2),
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(3),
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(4),
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(5),
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(13)
                                                      );
      
      SingleDigitDisplay disp2(
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(17),
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(27),
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(22),
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(6),
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(19)
                                                      );
      
      //set playerOneDisplay to 10 seconds
      disp1.setDisplay(1);
      disp2.setDisplay(0);
            
      //Initialize input
      //No input
      
      //Initialize output
      /*LED1 = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(x);
      LED2 = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(x);
      LED3 = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(x);
      LED4 = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(x);
      LED5 = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(x);
      LED6 = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(x);*/
      
      //buzzer = new Buzzer(((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(x));
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
      
      for(int i; i < 4; i++)
      {
            nanowait(500000);
            //buzz
      }
      
      returnState = this->stateManager->getState("StateWaitForOne");
      
      return returnState;
}

void StateWaitForOne::setup(void* data)
{
std::cout << "StateWaitForOne\n";
      ((GameData* ) data)->startTick = getTick();
      
      //Initialize input
      theButton = new Button(((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(21));
      
      //Initialize output
      this->playerOneDisplay1 = new SingleDigitDisplay(
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(2),
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(3),
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(4),
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(5),
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(13)
                                                      );
      
      this->playerOneDisplay2 = new SingleDigitDisplay(
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(17),
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(27),
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(22),
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(6),
                                                       ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(19)
                                                      );
                                                      
      this->playerOneDisplay1->setDecimal(HIGH);
      
      this->countdownLED = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(24);
      this->countdownLED->setValue(HIGH);
      this->countupLED = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(23);
      this->countupLED->setValue(LOW);
}

State* StateWaitForOne::process(void* data)
{
      State* returnState = this;
      GameData* gameData = (GameData*) data;
      int seconds = 0;
      int tenths = 0;
      
      //display the current countdown
      long tickDiff = getTick() - gameData->startTick;
      tickDiff = abs(10000 - tickDiff);
      
      seconds = tickDiff / 1000;
      tenths = ((tickDiff / 100) - seconds * 10);
      
      this->playerOneDisplay1->setDisplay(seconds);
      this->playerOneDisplay2->setDisplay(tenths);
      
      //set the count down/up leds
      
      //check if button pressed
      if(this->theButton->pressed())
      {
            gameData->endTick = getTick();
            returnState = this->stateManager->getState("StatePostGame");
      }
      
      if (tickDiff >= 10000)
      {
            returnState = this->stateManager->getState("StatePostGame");
      }
      
      return returnState;
}

void StatePostGame::setup(void* data)
{
      std::cout << "StatePostGame\n";
      
      //Initialize input
      this->startOver = new Button(((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(20));
      
      //Initialize output
      
     /* playerOneDisplay1 = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(x);
      playerOneDisplay2 = ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(x);
      */
}

State* StatePostGame::process(void* data)
{
      State* returnState = this;
      if(this->startOver->pressed()) returnState = this->stateManager->getState("StateBeforeGame");
      return returnState;
}
