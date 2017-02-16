#include "HW5_States.h"

State::State(StateManager* sm, PeripheralFactory* pf)
{
      this->stateManager = sm;
      this->peripheralFactory = pf;
}

StateManager::StateManager()
{
      
}

void StateManager::registerState(const char* stateName, State* state)
{
      stateMap[stateName] = state;
}

State* StateManager::getState(const char* stateName)
{
      if (stateMap.find(stateName) == stateMap.end())
      {
            return (State*) 0;
      }
      else
      {
            return stateMap[stateName];
      }
}
