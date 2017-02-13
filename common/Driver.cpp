#include "Driver.h"

Driver::Driver(StateManager* sm)
{
      this->stateManager = sm;
}

void Driver::run(void* data)
{
      State* currentState = this->stateManager->getState("StateInitial");
      
      while (currentState)
      {
            currentState = currentState->processInput(data);
      }
}
