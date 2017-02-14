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
      
      // initialize GPIO
      // display pins
      GPIOPin* pin = gpio->getPin(2);
      pin->setPull(DOWN);
      pin->setDirection(OUT);
      pin->setValue(LOW);
      
      pin = gpio->getPin(3);
      pin->setPull(DOWN);
      pin->setDirection(OUT);
      pin->setValue(LOW);

      pin = gpio->getPin(4);
      pin->setPull(DOWN);
      pin->setDirection(OUT);
      pin->setValue(LOW);
      
      pin = gpio->getPin(5);
      pin->setPull(DOWN);
      pin->setDirection(OUT);
      pin->setValue(LOW);
      
      //display 2
      /*
      pin = gpio->getPin(22);
      pin->setPull(DOWN);
      pin->setDirection(OUT);
      pin->setValue(LOW);
      
      pin = gpio->getPin(13);
      pin->setPull(DOWN);
      pin->setDirection(OUT);
      pin->setValue(LOW);
      
      pin = gpio->getPin(19);
      pin->setPull(DOWN);
      pin->setDirection(OUT);
      pin->setValue(LOW);
      
      pin = gpio->getPin(26);
      pin->setPull(DOWN);
      pin->setDirection(OUT);
      pin->setValue(LOW);
      */
      // initialize GPIO 21   ** :P  ** No extra points for making ALL group members to participate :P 
      pin = gpio->getPin(21);
      pin->setPull(DOWN);
      pin->setDirection(IN);
      
      std::cout << "current pin 21 value: " << pin->getValue() << "\n";
      
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
      this->theDisplay = new SingleDigitDisplay(
                                                      ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(2),
                                                      ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(3),
                                                      ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(4),
                                                      ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(5),
                                                      ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(6)
                                                      );
/*                                                      
      this->secDisplay = new SingleDigitDisplay(
                                                      ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(22),
                                                      ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(13),
                                                      ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(19),
                                                      ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(26),
                                                      ((GPIOSystem*)this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO))->getPin(6)
                                                      );
*/                                                      
//      this->theDisplay->setDisplay(1);
}

StateBeforeGame::~StateBeforeGame()
{
      delete this->theButton;
      delete this->theDisplay;
      //delete this->secDisplay;
}

State* StateBeforeGame::processInput(void* data)
{
      State* returnState = this;
      
      if (this->theButton->pressed())
      {
            this->pressCount++;
            
            /*
            //toggle the LED
            if (this->LED->getValue() == HIGH)
            {
                  this->LED->setValue(LOW);
            }
            else
            {
                  this->LED->setValue(HIGH);
            }
            */
            
            this->theDisplay->setDisplay(this->pressCount);
            //this->secDisplay->setDisplay(this->pressCount);
            
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
