#include "RecipeStates.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>


//Added this function from HW5. Wasn't sure if we needed it.
//returns the current number of milliseconds
long getTick()
{
	struct timespec ts;
	timespec_get(&ts, TIME_UTC);
	
	return (ts.tv_sec * 1000) + (ts.tv_nsec/1000000);
}

State* Initial::process(void* data)
{
	RecipeData* data = (RecipeData*) data;
}

State* Welcome::process(void* data)
{
	RecipeData* data = (RecipeData*) data;
}

State* RecipeList::process(void* data)
{
	RecipeData* data = (RecipeData*) data;
}

State* DisplayIngredients::process(void* data)
{
	RecipeData* data = (RecipeData*) data;
}

State* ZeroScale::process(void* data)
{
	RecipeData* data = (RecipeData*) data;
}

State* Fill::process(void* data)
{
	RecipeData* data = (RecipeData*) data;
}

State* Validate::process(void* data)
{
	RecipeData* data = (RecipeData*) data;
}

State* ErrorFill::process(void* data)
{
	RecipeData* data = (RecipeData*) data;
}

State* AdditionalStep::process(void* data)
{
	RecipeData* data = (RecipeData*) data;
}

State* Done::process(void* data)
{
	RecipeData* data = (RecipeData*) data;
}