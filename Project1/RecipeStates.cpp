#include "RecipeStates.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <cstdio>

#define		WELCOME_MESSAGE	"               WELCOME!\n   Start to choose a recipe\n               A - Start\n"
#define		DONE_MESSAGE	"Greate job!\n\nEnjoy your tasty meal.\nNom Nom Nom\n"
#define		MEASURE_EMPTY_MESSAGE	"Place empty %s container on scale.\n"
#define		MEASURE_FILL_MESSAGE	"Fill container with %s grams of %s.\n\n    0    grams\n"

//returns the current number of milliseconds
long getTick()
{
	struct timespec ts;
	timespec_get(&ts, TIME_UTC);
	
	return (ts.tv_sec * 1000) + (ts.tv_nsec/1000000);
}

State* Initial::process(void* data)
{
	State* returnState = this->stateManager->getState(WELCOME);
	GPIOSystem* gpio = (GPIOSystem*) this->peripheralFactory->getPeripheral(PERIPHERAL_GPIO);
	RecipeData* recipeData = (RecipeData*) data;

	//Initialize data
	//data->display = new Display(gpio->getPin(2)); 2-9
	recipeData->keypad = new KeyPad(gpio->getPin(10),
									gpio->getPin(11),
									gpio->getPin(12),
									gpio->getPin(13),
									gpio->getPin(14),
									gpio->getPin(15),
									gpio->getPin(16),
									gpio->getPin(17));

	recipeData->scale = new Scale(gpio->getPin(18), gpio->getPin(19));
	recipeData->currentRecipe = (Recipe*) 0;
	recipeData->recipes = new Recipes();
	
	// display the welcome message
	// data->display->print(WELCOME_MESSAGE);
	std::cout << WELCOME_MESSAGE;
	
	return returnState;
}

State* Welcome::process(void* data)
{
	State* returnState = this;
	RecipeData* recipeData = (RecipeData*) data;

	//Get keypad input
	int userInput = recipeData->keypad->getKey();

	switch(userInput)
	{
		case 1:
			returnState = this->stateManager->getState(RECIPELIST);
			break;
		default:
			//do nothing. wait for correct input.
			break;
	}

	return returnState;
}

void RecipeList::setup(void* data)
{
	RecipeData* recipeData = (RecipeData*) data;
	pageNumber = 0;
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
			recipeData->currentRecipe = recipes->getRecipe((pageNumber*4)+0);
			//returnState = this->stateManager->getState("DisplayIngredients" + recipe[1].getState());
			break;
		case 2:
			recipeData->currentRecipe = recipes->getRecipe((pageNumber*4)+1);
			//returnState = this->stateManager->getState("DisplayIngredients" + recipe[2].getState());
			break;
		case 3:
			recipeData->currentRecipe = recipes->getRecipe((pageNumber*4)+2);
			//returnState = this->stateManager->getState("DisplayIngredients" + recipe[3].getState());
			break;
		case 4:
			recipeData->currentRecipe = recipes->getRecipe((pageNumber*4)+3);
			//returnState = this->stateManager->getState("DisplayIngredients" + recipe[4].getState());
			break;
		case 10:
			//Scroll up if not at the top
			if(pageNumber == 0);
			else
			{
				pageNumber++;
			}
			break;
		case 13:
			//Scroll down if not at the bottom
			if(pageNumber == 2);
			else
			{
				pageNumber--;
			}
			break;
		default:
			//do nothing
			break;
	}

	return returnState;
}

State* DisplayIngredients::process(void* data)
{
	State* returnState = this;
	RecipeData* recipeData = (RecipeData*) data;

	if (recipeData->keypad->getKey() == 10)
	{
		// ready to move to measuring steps
		// display the first measuring step
		char msg[160];
		recipeData->currentMeasured = recipeData->currentRecipe->getMeasured(0);
		sprintf(msg, MEASURE_EMPTY_MESSAGE, recipeData->currentMeasured->ingredient);
		std::cout << msg;
		
		returnState = this->stateManager->getState(ZEROSCALE);
	}
	
	return returnState;
}

State* ZeroScale::process(void* data)
{
	State* returnState = this;
	RecipeData* recipeData = (RecipeData*) data;
	
	if (recipeData->keypad->getKey() == 10)
	{
		// ready to zero the scale
		// wait a moment to give the scale time to settle
		nanowait(0, 500000000);
		
		// now read scale until we see something reasonable.
		int reading = 0;
		
		while ((reading = recipeData->scale->getReading()) <= -1){}
		
		// set the offset
		recipeData->scale->setOffset(reading);
		
		// now transition to Fill state
		// display fill message
		char msg[160];
		sprintf(msg, MEASURE_FILL_MESSAGE, recipeData->currentMeasured->grams, recipeData->currentMeasured->ingredient);
		std::cout << msg;
		
		returnState = this->stateManager->getState(FILL);
	}

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

	if (recipeData->keypad->getKey() == 10)
	{
		char* step = recipeData->currentRecipe->getStep(this->stepNumber++);
		if (step)
		{
			// display the current step text
			// display->print(step);
		}
		else
		{
			// move to done state
			// display done message here
			// display->print(DONE_MESSAGE);
			std::cout << DONE_MESSAGE;
			returnState = this->stateManager->getState(DONE);
		}
	}
	
	return returnState;
}

State* Done::process(void* data)
{
	State* returnState = this;
	RecipeData* recipeData = (RecipeData*) data;

	// if the A button pressed to 60 second time out go back to WELCOME
	if (recipeData->keypad->getKey() == 10 ||
		(getTick() - startTick) > 60000)
	{
		// display welcome message code goes here
		std::cout << WELCOME_MESSAGE;
		returnState = this->stateManager->getState(WELCOME);
	}
	
	return returnState;
}

void Done::setup(void* data)
{
	startTick = getTick();
}
