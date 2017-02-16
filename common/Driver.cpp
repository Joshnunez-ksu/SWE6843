#include "Driver.h"

Driver::Driver(StateManager* sm)
{
      this->stateManager = sm;
}

void Driver::run(void* data)
{
      State* currentState = this->stateManager->getState("StateInitial");
      State* lastState;
      
      while (currentState)
      {
            if (lastState != currentState)
            {
                  currentState->setup(data);
            }
            
            lastState = currentState;
            currentState = currentState->process(data);
      }
}
