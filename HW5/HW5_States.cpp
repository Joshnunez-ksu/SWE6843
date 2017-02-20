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
      GameData* gameData = (GameData*) data;
      
      //Initialize game buttons
      gameData->startButton = new Button(gpio->getPin(20));
      gameData->playerOneButton = new Button(gpio->getPin(21));
      gameData->playerTwoButton = new Button(gpio->getPin(16));
      
      //Initialize display
      gameData->playerOneDisplay1 = new SingleDigitDisplay(gpio->getPin(2), gpio->getPin(3), gpio->getPin(4), gpio->getPin(5),
                                                          gpio->getPin(13));
      gameData->playerOneDisplay2 = new SingleDigitDisplay(gpio->getPin(17), gpio->getPin(27), gpio->getPin(22), gpio->getPin(6),
                                                          gpio->getPin(13));
      gameData->playerTwoDisplay1 = new SingleDigitDisplay(gpio->getPin(19), gpio->getPin(18), gpio->getPin(26), gpio->getPin(24),
                                                          gpio->getPin(13));
      gameData->playerTwoDisplay2 = new SingleDigitDisplay(gpio->getPin(10), gpio->getPin(9), gpio->getPin(11), gpio->getPin(8),
                                                          gpio->getPin(13));
      
      //Initialize LEDs
      gameData->playerOneLED = gpio->getPin(14);
      gameData->playerTwoLED = gpio->getPin(15);
      gameData->countUpDownLED = gpio->getPin(23);
      
      //Initialize buzzer
      gameData->buzzer = new SoftBuzzer(gpio->getPin(12));
}

State* StateInitial::process(void* data)
{
      GameData* gameData = (GameData*) data;
      
      //set playerOneDisplay to 10 seconds
      gameData->playerOneDisplay1->setDisplay(1);
      gameData->playerOneDisplay2->setDisplay(0);
      gameData->playerOneDisplay1->setDecimal(HIGH);
      
      //set playerTwoDisplay to 10 seconds
      gameData->playerTwoDisplay1->setDisplay(1);
      gameData->playerTwoDisplay2->setDisplay(0);
      
      State* returnState = this;
      returnState = this->stateManager->getState("StateBeforeGame");
      return returnState;
}

void StateBeforeGame::setup(void* data)
{     
      std::cout << "StateBeforeGame\n"; 
}

State* StateBeforeGame::process(void* data)
{
      GameData* gameData = (GameData*) data;
      State* returnState = this;
      
      if(gameData->playerOneButton->pressed())
      {
            returnState = (State*) 0;
            delete gameData->startButton;
            delete gameData->playerOneButton;
            delete gameData->playerTwoButton;
            delete gameData->playerOneDisplay1;
            delete gameData->playerOneDisplay2;
            delete gameData->playerTwoDisplay1;
            delete gameData->playerTwoDisplay2;
            //delete gameData->ledTree;
            delete gameData->buzzer;
      }
      else if(gameData->startButton->pressed())
      {
            //std::cout << "BeforeGame button press\n";
            returnState = this->stateManager->getState("StatePreInGame");
      }
      
      return returnState;
}

void StatePreInGame::setup(void* data)
{
      std::cout << "StatePreInGame\n";
      GameData* gameData = (GameData*) data;
      
      gameData->startTick = 0;
      gameData->playerOneTick = 0;
      gameData->playerTwoTick = 0;

      //Count up is low, Count down is high
      gameData->countUpDownLED->setValue(LOW);
}

State* StatePreInGame::process(void* data)
{
      GameData* gameData = (GameData*) data;
      State* returnState = this;
      
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
      
      gameData->playerOneLED->setDirection(OUT);
      gameData->playerOneLED->setValue(LOW);
      gameData->playerTwoLED->setDirection(OUT);
      gameData->playerTwoLED->setValue(LOW);
      
      gameData->countUpDownLED->setDirection(OUT);
      gameData->countUpDownLED->setValue(LOW);
      
      for(int i=0; i < 4; i++)
      {
            gameData->buzzer->setFrequency(440);
            gameData->buzzer->enable(500);
            nanowait(0, 500000000);
      }
      
      gameData->startTick = getTick();
      returnState = this->stateManager->getState("StateWaitForTwo");
      
      return returnState;
}

void StateWaitForOne::setup(void* data)
{
      std::cout << "StateWaitForOne\n";
      GameData* gameData = (GameData*) data;
      //gameData->playerOneDisplay1->setDecimal(HIGH);
}

State* StateWaitForOne::process(void* data)
{
      GameData* gameData = (GameData*) data;
      State* returnState = this;
      
      int seconds = 0;
      int tenths = 0;
      
      long tickDiff = getTick() - gameData->startTick;
      int displayValue = abs(10000 - tickDiff);
      
      seconds = displayValue / 1000;
      tenths = ((displayValue / 100) - seconds * 10);
      
      if(10000 < tickDiff)
      {
            gameData->countUpDownLED->setValue(HIGH);
      }
      
      if(gameData->playerOneTick == 0)
      {
            gameData->playerOneDisplay1->setDisplay(seconds);
            gameData->playerOneDisplay2->setDisplay(tenths);
            if(gameData->playerOneButton->pressed())
            {
                  gameData->playerOneTick = getTick();
                  returnState = this->stateManager->getState("StatePostGame");
            }
      }
      else if(gameData->playerTwoTick == 0)
      {
            gameData->playerTwoDisplay1->setDisplay(seconds);
            gameData->playerTwoDisplay2->setDisplay(tenths);
            if(gameData->playerTwoButton->pressed())
            {
                  gameData->playerTwoTick = getTick();
                  returnState = this->stateManager->getState("StatePostGame");
            }
      }
      
      if (tickDiff >= 20000)
      {
            returnState = this->stateManager->getState("StatePostGame");
      }
      
      return returnState;
}

void StateWaitForTwo::setup(void* data)
{
      std::cout << "StateWaitForTwo\n";
}

State* StateWaitForTwo::process(void* data)
{
      GameData* gameData = (GameData*) data;
      State* returnState = this;
      
      int seconds = 0;
      int tenths = 0;
      
      //display the current countdown
      long tickDiff = getTick() - gameData->startTick;
      int displayValue = abs(10000 - tickDiff);
      
      seconds = displayValue / 1000;
      tenths = ((displayValue / 100) - seconds * 10);
      
      gameData->playerOneDisplay1->setDisplay(seconds);
      gameData->playerOneDisplay2->setDisplay(tenths);
      
      gameData->playerTwoDisplay1->setDisplay(seconds);
      gameData->playerTwoDisplay2->setDisplay(tenths);
      
      if(10000 < tickDiff && gameData->countUpDownLED->getValue() != HIGH)
      {
            gameData->countUpDownLED->setValue(HIGH);
      }
      
      //check if button pressed
      if(gameData->playerOneButton->pressed())
      {
            gameData->playerOneTick = getTick();
            returnState = this->stateManager->getState("StateWaitForOne");
      }
      
      if(gameData->playerTwoButton->pressed())
      {
            gameData->playerTwoTick = getTick();
            returnState = this->stateManager->getState("StateWaitForOne");
      }
      
      if (tickDiff >= 20000)
      {
            returnState = this->stateManager->getState("StatePostGame");
      }
      
      return returnState;
}

void StatePostGame::setup(void* data)
{
      std::cout << "StatePostGame\n";
}

State* StatePostGame::process(void* data)
{
      GameData* gameData = (GameData*) data;
      State* returnState = this;
      
      //indicate winner
      long playerOneTime = abs(gameData->playerOneTick - gameData->startTick);
      long playerTwoTime = abs(gameData->playerTwoTick - gameData->startTick);
      
      if (playerOneTime < playerTwoTime)
      {
            gameData->playerOneLED->setValue(HIGH);
      }
      else if (playerTwoTime > playerOneTime)
      {
            gameData->playerTwoLED->setValue(HIGH);
      }
      else
      {
            gameData->playerOneLED->setValue(HIGH);
            gameData->playerTwoLED->setValue(HIGH);
      }
      
      if(gameData->startButton->pressed()) returnState = this->stateManager->getState("StateBeforeGame");
      
      return returnState;
}
