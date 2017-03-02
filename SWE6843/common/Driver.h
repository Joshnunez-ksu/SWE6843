#ifndef __DRIVER_H__
#define __DRIVER_H__

#include "State.h"

class Driver
{
public:
      Driver(StateManager* sm);
      void run(void* data);
      
private:
      StateManager* stateManager;
};

#endif
