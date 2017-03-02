/********************************************************************************
Project One
********************************************************************************/

#include "../common/Driver.h"
#include "RecipeStates.h"

int main(int argc, char* args[])
{
      RecipeData theData;
      
      PeripheralFactory	pf;
      StateManager		sm;
      
      sm.registerState("StateInitial",		new Initial(&sm, &pf));
      sm.registerState("Welcome",			new Welcome(&sm, &pf));
      sm.registerState("RecipeList",		new RecipeList(&sm, &pf));
      sm.registerState("DisplayIngredients",new DisplayIngredients(&sm, &pf));
      sm.registerState("ZeroScale",			new ZeroScale(&sm, &pf));
      sm.registerState("Fill",				new Fill(&sm, &pf));
      sm.registerState("Validate",			new Validate(&sm, &pf));
      sm.registerState("ErrorFill",			new ErrorFill(&sm, &pf));
      sm.registerState("AdditionalStep",	new AdditionalStep(&sm, &pf));
      sm.registerState("Done",				new Done(&sm, &pf));

      Driver driver(&sm);
      driver.run(&theData);
      
      return 0;
}
