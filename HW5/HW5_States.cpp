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
      gameData->playerOneLED = new GPIOPin(gpio->getPin(14));
      gameData->playerTwoLED = new GPIOPin(gpio->getPin(15));
      gameData->countUpLED = new GPIOPin(gpio->getPin(23));
      gameData->countDownLED = new GPIOPin(gpio->getPin(24));
      gameData->ledTree = nullptr;/*{ new GPIOPin(),
                            new GPIOPin(),
                            new GPIOPin(),
                            new GPIOPin(),
                            new GPIOPin() };*/
      //Initialize buzzer
      gameData->buzzer = new Buzzer(gpio->getPin(12));
}

State* StateInitial::process(void* data)
{
      GameData* gameData = (GameData*) data;
      
      //set playerOneDisplay to 10 seconds
      gameData->playerOneDisplay1->setDisplay(1);
      gameData->playerOneDisplay2->setDisplay(0);
      
      //set playerTwoDisplay to 10 seconds
      gameData->playerTwoDisplay2->setDisplay(1);
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
            delete gameData->playerOneLED;
            delete gameData->playerTwoLED;
            delete gameData->countUpLED;
            delete gameData->countDownLED;
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
      gameData->countDownLED->setValue(HIGH);
}

State* StatePreInGame::process(void* data)
{
      GameData* gameData = (GameData*) data;
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
      GameData* gameData = (GameData*) data;
      //gameData->startTick = getTick();
                                                      
      //gameData->playerOneDisplay1->setDecimal(HIGH);
}

State* StateWaitForOne::process(void* data)
{
      GameData* gameData = (GameData*) data;
      State* returnState = this;
      
      int seconds = 0;
      int tenths = 0;
      
      long trueTickDiff = getTick() - gameData->startTick; //To switch the countUpLED & countDownLED
      long tickDiff = getTick() - gameData->startTick;
      tickDiff = abs(10000 - tickDiff);
      
      seconds = tickDiff / 1000;
      tenths = ((tickDiff / 100) - seconds * 10);
      
      if(trueTickDiff <= 0 && gameData->countDownLED->getValue() != LOW)
      {
            gameData->countDownLED->setValue(LOW);
      }
      
      if(gameData->playerOneTick == 0)
      {
            gameData->playerOneDisplay1->setDisplay(seconds);
            gameData->playerOneDisplay2->setDisplay(tenths);
            if(gameData->playerOneButton->pressed())
            {
                  gameData->endTick = getTick();
                  returnState = this->stateManager->getState("StatePostGame");
            }
      }
      else if(gameData->playerTwoTick == 0)
      {
            gameData->playerTwoDisplay1->setDisplay(seconds);
            gameData->playerTwoDisplay2->setDisplay(tenths);
            if(gameData->playerTwoButton->pressed())
            {
                  gameData->endTick = getTick();
                  returnState = this->stateManager->getState("StatePostGame");
            }
      }
      
      if (tickDiff >= 10000)
      {
            returnState = this->stateManager->getState("StatePostGame");
      }
      
      return returnState;
}

void StateWaitForTwo::setup(void* data)
{
      std::cout << "StateWaitForTwo\n";
      GameData* gameData = (GameData*) data;
      gameData->startTick = getTick();
                                                      
      gameData->playerOneDisplay1->setDecimal(HIGH);
      gameData->countDownLED-setValue(HIGH);
}

State* StateWaitForTwo::process(void* data)
{
      GameData* gameData = (GameData*) data;
      State* returnState = this;
      
      int seconds = 0;
      int tenths = 0;
      
      //display the current countdown
      long tickDiff = getTick() - gameData->startTick;
      long trueTickDiff = getTick() - gameData->startTick; //To switch the countUpLED & countDownLED
      tickDiff = abs(10000 - tickDiff);
      
      seconds = tickDiff / 1000;
      tenths = ((tickDiff / 100) - seconds * 10);
      
      gameData->playerOneDisplay1->setDisplay(seconds);
      gameData->playerOneDisplay2->setDisplay(tenths);
      
      gameData->playerTwoDisplay1->setDisplay(seconds);
      gameData->playerTwoDisplay2->setDisplay(tenths);
      
      if(trueTickDiff <= 0 && gameData->countDownLED->getValue() != LOW)
      {
            gameData->countDownLED->setValue(LOW);
      }
      
      //check if button pressed
      if(gameData->playerOneButton->pressed())
      {
            gameData->playerOneTick = getTick();
            returnState = this->stateManager->getState("StateWaitForOne");
      }
      else if(gameData->playerTwoButton->pressed())
      {
            gameData->playerTwoTick = getTick();
            returnState = this->stateManager->getState("StateWaitForOne");
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
}

State* StatePostGame::process(void* data)
{
      GameData* gameData = (GameData*) data;
      State* returnState = this;
      if(gameData->startButton->pressed()) returnState = this->stateManager->getState("StateBeforeGame");
      return returnState;
}
