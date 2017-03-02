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
	State* returnState = this;
	GPIOSystem* gpio = (GPIOSystem*) this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO);
	RecipeData* recipeData = (RecipeData*) data;

	//Initialize peripherals
	//data->display = new Display(gpio->getPin(2));
	//data->keypad = new Keypad(gpio->getPin(2));
	//data->scale = new Scale(gpio->getPin(2));
	//data->currentRecipe = ??;
	
	recipeData->recipes = new Recipes();
	
	std::cout << "Recipes:\n";
	
	int i=0;
	
	while (recipeData->currentRecipe = recipeData->recipes->getRecipe(i))
	{
		std::cout << "\t" << i << ") " << recipeData->currentRecipe->getName() << "\n";
		
		i++;
	}

	return (State*) 0;
	//return returnState;
}

State* Welcome::process(void* data)
{
	State* returnState = this;
	RecipeData* recipeData = (RecipeData*) data;

	//Get keypad input
	int userInput = data->keypad->getKey();

	switch(userInput)
	{
		case 1:
			returnState = this->stateManager->getState("RecipeList");
			break;
		default:
			//do nothing. wait for correct input.
			break;
	}

	return returnState;
}

State* RecipeList::process(void* data)
{
	State* returnState = this;
	RecipeData* recipeData = (RecipeData*) data;

	//Show recipes
	//data->display->setValue(recipes);

	//Get keypad input
	int userInput = recipeData->keypad->getKey();

	switch(userInput)
	{
		case 0:
			break;
		case 1:
			//returnState = this->stateManager->getState("DisplayIngredients" + recipe[1].getState());
			break;
		case 2:
			//returnState = this->stateManager->getState("DisplayIngredients" + recipe[2].getState());
			break;
		case 3:
			//returnState = this->stateManager->getState("DisplayIngredients" + recipe[3].getState());
			break;
		case 4:
			//returnState = this->stateManager->getState("DisplayIngredients" + recipe[4].getState());
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		case 10:
			//Scroll up if not at the top
			//clear the display
			//data->display->clear();
			//show the recipes
			//data->display->print("recipes");
			break;
		case 11:
			break;
		case 12:
			break;
		case 13:
			//Scroll down if not at the bottom
			//clear the display
			//data->display->clear();
			//show the recipes
			//data->display->print("recipes");
			break;
	}

	return returnState;
}

State* DisplayIngredients::process(void* data)
{
	State* returnState = this;
	RecipeData* recipeData = (RecipeData*) data;

	return returnState;
}

State* ZeroScale::process(void* data)
{
	State* returnState = this;
	RecipeData* recipeData = (RecipeData*) data;

	return returnState;
}

State* Fill::process(void* data)
{
	State* returnState = this;
	RecipeData* recipeData = (RecipeData*) data;

	return returnState;
}

State* Validate::process(void* data)
{
	State* returnState = this;
	RecipeData* recipeData = (RecipeData*) data;

	return returnState;
}

State* ErrorFill::process(void* data)
{
	State* returnState = this;
	RecipeData* recipeData = (RecipeData*) data;

	return returnState;
}

State* AdditionalStep::process(void* data)
{
	State* returnState = this;
	RecipeData* recipeData = (RecipeData*) data;

	return returnState;
}

State* Done::process(void* data)
{
	State* returnState = this;
	RecipeData* recipeData = (RecipeData*) data;

	return returnState;
}
