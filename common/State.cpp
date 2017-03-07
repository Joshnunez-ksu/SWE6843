#include "State.h"

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
	std::string* name = new std::string(stateName);
      stateMap[*name /*stateName*/] = state;
	delete name;
}

State* StateManager::getState(const char* stateName)
{
	State* returnValue = (State*) 0;
	std::string* name = new std::string(stateName);
	
	if (stateMap.find(*name /*stateName*/) != stateMap.end())
	{
		returnValue = stateMap[*name /*stateName*/];
	}

	//delete name;

	return returnValue;
}
