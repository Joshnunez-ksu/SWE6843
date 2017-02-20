/********************************************************************************
Homework 5

Josh Nunez
Josh Osburn

********************************************************************************/

#include "../common/Driver.h"
#include "HW5_States.h"

int main(int argc, char* args[])
{
      bool twoPlayer = false;
      
      if (argc > 1)
      {
            if (*args[1] == '2')
            {
                  twoPlayer = true;
            }
      }

      GameData theData;
      
      PeripheralFactory pf;
      StateManager stateManager;
      
      stateManager.registerState("StateInitial", new StateInitial(&stateManager, &pf));
      stateManager.registerState("StateBeforeGame", new StateBeforeGame(&stateManager, &pf));
      stateManager.registerState("StatePreInGame", new StatePreInGame(&stateManager, &pf));
      stateManager.registerState("StateWaitForOne", new StateWaitForOne(&stateManager, &pf));
      
      if (!twoPlayer)
      {
            //single player mode
            stateManager.registerState("StateWaitForTwo", stateManager.getState("StateWaitForOne"));
            theData.playerOneTick = 0;
            theData.playerTwoTick = -1;
      }
      else
      {
            //two player mode
            stateManager.registerState("StateWaitForTwo", new StateWaitForTwo(&stateManager, &pf));
            theData.playerOneTick = 0;
            theData.playerTwoTick = 0;
      }
      
      stateManager.registerState("StatePostGame", new StatePostGame(&stateManager, &pf));

      Driver driver(&stateManager);
      driver.run(&theData);
      
      return 0;
}
