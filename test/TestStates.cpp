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
      
      std::cout << "End StateInitial\n";
      
      return this->stateManager->getState("StateBeforeGame");
}

State* StateBeforeGame::processInput(void* data)
{
      std::cout << "Start StateBeforeGame\n";
      
      GameData* gameData = (GameData*) data;
      gameData->startTick = getTick();
      
      timespec ts;
      ts.tv_sec = 3;
      ts.tv_nsec = 0;
      nanosleep(&ts, &ts);
      
      std::cout << "End StateBeforeGame\n";
      
      return this->stateManager->getState("StateAfterGame");
}

State* StateAfterGame::processInput(void* data)
{
      std::cout << "Start StateAfterGame\n";
      
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
