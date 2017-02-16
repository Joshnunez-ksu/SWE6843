#ifndef __STATE_H__
#define __STATE_H__

#include "Peripherals.h"
#include <map>
#include <string>

class StateManager;

class State
{
public:
      State(StateManager* sm, PeripheralFactory* pf);
      virtual void setup();
      virtual State* process(void* data) = 0;

protected:
      StateManager* stateManager;
      PeripheralFactory* peripheralFactory;
};

class StateManager
{
public:
      StateManager();
      void registerState(const char* stateName, State* state);
      State* getState(const char* stateName);

private:
      std::map<std::string, State*> stateMap;
      
};

#endif
