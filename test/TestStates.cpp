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
      
State* StateInitial::process(void* data)
{
      std::cout << "Start StateInitial\n";
      
      GameData* gameData = (GameData*) data;
      
      gameData->startTick = 0;
      gameData->playerOneTick = 0;
      gameData->playerTwoTick = 0;

      GPIOSystem* gpio = (GPIOSystem*) this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO);

      gameData->displayOne = new SingleDigitDisplay(gpio->getPin(2), gpio->getPin(3), gpio->getPin(4), gpio->getPin(5), gpio->getPin(13));
      gameData->displayTwo = new SingleDigitDisplay(gpio->getPin(17), gpio->getPin(27), gpio->getPin(22), gpio->getPin(6), gpio->getPin(13));
      gameData->displayThree = new SingleDigitDisplay(gpio->getPin(10), gpio->getPin(9), gpio->getPin(11), gpio->getPin(8), gpio->getPin(13));
      gameData->displayFour = new SingleDigitDisplay(gpio->getPin(19), gpio->getPin(18), gpio->getPin(26), gpio->getPin(24), gpio->getPin(13));
      
      gameData->buttonOne = new Button(gpio->getPin(20));
      gameData->buttonTwo = new Button(gpio->getPin(21));
      gameData->buttonThree = new Button(gpio->getPin(16));
      
      gameData->LED1 = gpio->getPin(23);
      gameData->LED1->setDirection(OUT);
      gameData->LED1->setValue(LOW);
      
      gameData->LED2 = gpio->getPin(14);
      gameData->LED2->setDirection(OUT);
      gameData->LED2->setValue(LOW);
      
      gameData->LED3 = gpio->getPin(15);
      gameData->LED3->setDirection(OUT);
      gameData->LED3->setValue(HIGH);
      
      gameData->buzzer = new SoftBuzzer(gpio->getPin(12));
      gameData->buzzer->setFrequency(440);
      gameData->buzzer->enable(3000);
      
      std::cout << "End StateInitial\n";
      
      ((GameData*) data)->startTick = getTick();
      
      std::cout << "Starting StateBeforeGame\n";
      return this->stateManager->getState("StateBeforeGame");
}

void StateInitial::setup(void* data)
{
}

StateBeforeGame::StateBeforeGame(StateManager* sm, PeripheralFactory* pf) : State(sm, pf)
{
      this->pressCount = 0;
}

void StateBeforeGame::setup(void* data)
{
}

StateBeforeGame::~StateBeforeGame()
{
}

State* StateBeforeGame::process(void* data)
{
      State* returnState = this;
      
      GameData* gameData = (GameData*) data;
      
      if (gameData->buttonOne->pressed() ||
           gameData->buttonTwo->pressed() ||
           gameData->buttonThree->pressed())
      {
            this->pressCount++;
            
            //toggle the LED
            if (gameData->LED1->getValue() == HIGH)
            {
                  gameData->LED1->setValue(LOW);
                  gameData->LED2->setValue(LOW);
                  gameData->LED3->setValue(HIGH);
            }
            else
            {
                  gameData->LED1->setValue(HIGH);
                  gameData->LED2->setValue(HIGH);
                  gameData->LED3->setValue(LOW);
            }
            
            gameData->displayOne->setDisplay(this->pressCount);
            gameData->displayTwo->setDisplay(this->pressCount);
            gameData->displayThree->setDisplay(this->pressCount);
            gameData->displayFour->setDisplay(this->pressCount);
            
            std::cout << "\rPressed " << this->pressCount << " times.";
            std::cout.flush();
      }

      // after 10 button presses change to the state
      if (this->pressCount >= 10)
      {
            std::cout << "\nEnd StateBeforeGame\n";
            std::cout << "Starting StateAfterGame\n";
            returnState = this->stateManager->getState("StateAfterGame");
      }
      
      return returnState;
}

void StateAfterGame::setup(void* data)
{
}

State* StateAfterGame::process(void* data)
{
      GameData* gameData = (GameData*) data;
      std::cout << "Start Tick = " << gameData->startTick << "\n";
      std::cout << "Tick Diff = " << getTick() - gameData->startTick << "\n";
      
      timespec ts;
      ts.tv_sec = 3;
      ts.tv_nsec = 0;
      nanosleep(&ts, &ts);
      
      GameData* gd = (GameData*) data;
      delete gd->displayOne;
      delete gd->displayTwo;
      delete gd->displayThree;
      delete gd->displayFour;
      
      delete gd->buttonOne;
      delete gd->buttonTwo;
      delete gd->buttonThree;
      
      delete gd->buzzer;
      std::cout << "End StateAfterGame\n";
      
      return (State*) 0;
}
