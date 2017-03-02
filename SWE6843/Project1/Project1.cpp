/********************************************************************************
Project One

To compile:
	g++ -o p1 Project1.cpp "RecipeStates.cpp" "Recipes.cpp" ../common/*.cpp

********************************************************************************/

#include "../common/Driver.h"
#include "RecipeStates.h"

int main(int argc, char* args[])
{
      RecipeData theData;
      
      PeripheralFactory	pf;
      StateManager		sm;
      
      sm.registerState(INITIAL,				new Initial(&sm, &pf));
      sm.registerState(WELCOME,				new Welcome(&sm, &pf));
      sm.registerState(RECIPELIST,			new RecipeList(&sm, &pf));
      sm.registerState(DISPLAYINGREDIENTS,	new DisplayIngredients(&sm, &pf));
      sm.registerState(ZEROSCALE,			new ZeroScale(&sm, &pf));
      sm.registerState(FILL,				new Fill(&sm, &pf));
      sm.registerState(VALIDATE,			new Validate(&sm, &pf));
      sm.registerState(ERRORFILL,			new ErrorFill(&sm, &pf));
      sm.registerState(ADDITIONALSTEP,		new AdditionalStep(&sm, &pf));
      sm.registerState(DONE,				new Done(&sm, &pf));

      Driver driver(&sm);
      driver.run(&theData);
      
      return 0;
}
