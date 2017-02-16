/********************************************************************************
Homework 5

Josh Nunez
Josh Osburn

********************************************************************************/

#include "HW5_States.h";

int main()
{
      PeripheralFactory pf;
      StateManager stateManager;
      
      stateManager.registerState("StateInitial", new StateInitial(&stateManager, &pf));
      stateManager.registerState("StateBeforeGame", new StateBeforeGame(&stateManager, &pf));
      stateManager.registerState("StatePreInGame", new StatePreInGame(&stateManager, &pf));
      stateManager.registerState("StateWaitForOne", new StateWaitForOne(&stateManager, &pf));
      stateManager.registerState("StatePostGame", new StatePostGame(&stateManager, &pf));
            
      GameData theData;

      Driver driver(&stateManager);
      driver.run(&theData);
      
      return 0;
}
