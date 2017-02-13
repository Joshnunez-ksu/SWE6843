#include <iostream>
#include "../common/Peripherals.h"
#include "../common/State.h"
#include "../common/Driver.h"

#include "TestStates.h"

int main(int args, char* arg[])
{
      PeripheralFactory pf;
      StateManager stateManager;
      
      stateManager.registerState("StateInitial", new StateInitial(&stateManager, &pf));
      stateManager.registerState("StateBeforeGame", new StateBeforeGame(&stateManager, &pf));
      stateManager.registerState("StateAfterGame", new StateAfterGame(&stateManager, &pf));
      
      if (args > 1 && *arg[1] == '1')
      {
            std::cout << "\nSingle Player\n";
      }
      
      GameData theData;

      Driver driver(&stateManager);
      driver.run(&theData);
      
      return 0;
}
