#include "RecipeStates.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <cstdio>

#define		WELCOME_MESSAGE			"               WELCOME!\n          Start to choose a recipe\n\n               A - Start\n"
#define		DONE_MESSAGE			"Greate job!\n\nEnjoy your tasty meal.\nNom Nom Nom\n"
#define		MEASURE_EMPTY_MESSAGE	"Place empty %s container on scale.\n\n               Press A when done\n"
#define		MEASURE_FILL_MESSAGE	"Fill container with %d grams of %s.\n\n    %d    grams\n"
#define		RECIPELIST_MESSAGE		"1. %-20s %+15s\n2. %s\n3. %s\n4. %-20s %+15s\n"
#define		ADDITIONAL_STEPS_MESSAGE	"%s\n          Press A\n"
#define		INGREDIENTS_MESSAGE		"%s\n     A - Done\n"
#define		CLEAR_SCALE_MESSAGE		"Remove %s and prepare to measure %s"

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
	recipeData->display = new Display(	gpio->getPin(20),
						gpio->getPin(25), 
						gpio->getPin(16), 
						gpio->getPin(21), 
						gpio->getPin(8), 
						gpio->getPin(15), 
						gpio->getPin(7), 
						gpio->getPin(18), 
						gpio->getPin(1), 
						gpio->getPin(23), 
						gpio->getPin(12), 
						gpio->getPin(24));
	
	recipeData->keypad = new KeyPad(gpio->getPin(5),
					gpio->getPin(6),
					gpio->getPin(26),
					gpio->getPin(19),
					gpio->getPin(13),
					gpio->getPin(9),
					gpio->getPin(10),
					gpio->getPin(22));

	recipeData->scale = new Scale(gpio->getPin(17), gpio->getPin(4));
	recipeData->currentRecipe = (Recipe*) 0;
	recipeData->recipes = new Recipes();
	
	// display the welcome message
	char msg[160];
	sprintf(msg, WELCOME_MESSAGE);
	recipeData->display->write(msg, 160);
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
		case 10:
			// display the first page of recipes
			char msg[160];
			sprintf(msg, RECIPELIST_MESSAGE, recipeData->recipes->getRecipe(0)->getName(), "", 
												recipeData->recipes->getRecipe(1)->getName(), 
												recipeData->recipes->getRecipe(2)->getName(), 
												recipeData->recipes->getRecipe(3)->getName(), "D - Next");
			msg[159] = 0;
			recipeData->display->write(msg, 160);
			
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
	this->pageNumber = 0;
}

State* RecipeList::process(void* data)
{
	State* returnState = this;
	RecipeData* recipeData = (RecipeData*) data;

	//Get keypad input
	int userInput = recipeData->keypad->getKey();

	switch(userInput)
	{
		case 1:
		case 2:
		case 3:
		case 4:
			std::cout << "selection: " << (pageNumber*2)+(userInput-1) << "\n";
			recipeData->currentRecipe = recipeData->recipes->getRecipe((pageNumber*2)+(userInput-1));
			
			char msg[160];
			sprintf(msg, INGREDIENTS_MESSAGE, recipeData->currentRecipe->getIngredientString(0));
			recipeData->display->write(msg, 160);
			
			returnState = this->stateManager->getState(DISPLAYINGREDIENTS);
			recipeData->measuredIndex = 0;
			break;
		case 10:
			//Scroll up if not at the top
			std::cout << "Scroll up, Page number: " << this->pageNumber << "\n";
			if(this->pageNumber == 1)
			{
				char msg[160];
				sprintf(msg, RECIPELIST_MESSAGE, recipeData->recipes->getRecipe(0)->getName(), "", 
												recipeData->recipes->getRecipe(1)->getName(), 
												recipeData->recipes->getRecipe(2)->getName(), 
												recipeData->recipes->getRecipe(3)->getName(), "D - Next");
				msg[159] = 0;
				recipeData->display->write(msg, 160);
				this->pageNumber--;
			}
			break;
		case 13:
			//Scroll down if not at the bottom
			std::cout << "Scroll down, Page number: " << this->pageNumber << "\n";
			if(this->pageNumber == 0)
			{
				char msg[160];
				sprintf(msg, RECIPELIST_MESSAGE, recipeData->recipes->getRecipe(4)->getName(), "A - Prev", 
												recipeData->recipes->getRecipe(5)->getName(), 
												"", 
												"", "");
				msg[159] = 0;
				recipeData->display->write(msg, 160);
				
				this->pageNumber++;
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
		recipeData->display->write(msg, 160);
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
		sprintf(msg, MEASURE_FILL_MESSAGE, recipeData->currentMeasured->grams, recipeData->currentMeasured->ingredient, 0);
		std::cout << msg;
		recipeData->display->write(msg, 160);
		
		returnState = this->stateManager->getState(FILL);
	}

	return returnState;
}

State* Fill::process(void* data)
{
	State* returnState = this;
	RecipeData* recipeData = (RecipeData*) data;
	
	float currentGrams = recipeData->scale->getGrams();
	
	if (currentGrams > -1)
	{
		// update the display
		char msg[160];
		sprintf(msg, MEASURE_FILL_MESSAGE, recipeData->currentMeasured->grams, recipeData->currentMeasured->ingredient, (int) currentGrams);
		std::cout << msg;
		recipeData->display->write(msg, 160);
		
		// check to see if this is over the measured limit
		if (currentGrams >= recipeData->currentMeasured->grams)
		{
			// check one more times...
			float checkGrams = 0;
			while ((checkGrams = recipeData->scale->getGrams()) <= -1){}
			
			if (checkGrams >= recipeData->currentMeasured->grams)
			{
				Measured* prevMeasured = recipeData->currentMeasured;
				
				// ok they have it ... get the next measured
				if((recipeData->currentMeasured = recipeData->currentRecipe->getMeasured(++recipeData->measuredIndex)))
				{
					// they need to clear the scale and move to the next measured...
					sprintf(msg, CLEAR_SCALE_MESSAGE, prevMeasured->ingredient, recipeData->currentMeasured->ingredient);
					recipeData->display->write(msg, 160);
					returnState = this->stateManager->getState(VALIDATE);
				}
				else
				{
					sprintf(msg, ADDITIONAL_STEPS_MESSAGE, recipeData->currentRecipe->getStep(0));
					recipeData->display->write(msg, 160);
					returnState = this->stateManager->getState(ADDITIONALSTEP);
				}
			} 
		}
	}

	return returnState;
}

State* Validate::process(void* data)
{
	State* returnState = this;
	RecipeData* recipeData = (RecipeData*) data;

	if (recipeData->scale->getGrams() == 0)
	{
		// continue to weighing the next measured
		char msg[160];
		sprintf(msg, MEASURE_FILL_MESSAGE, recipeData->currentMeasured->grams, recipeData->currentMeasured->ingredient, 0);
		recipeData->display->write(msg, 160);
		
		returnState = this->stateManager->getState(FILL);
	}
	
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
		char* step = recipeData->currentRecipe->getStep(++this->stepNumber);
		if (step)
		{
			// display the current step text
			char msg[160];
			sprintf(msg, ADDITIONAL_STEPS_MESSAGE, step);
			recipeData->display->write(msg, 160);
		}
		else
		{
			// move to done state
			std::cout << DONE_MESSAGE;
			recipeData->display->write(DONE_MESSAGE, 160);
			returnState = this->stateManager->getState(DONE);
		}
	}
	
	return returnState;
}

State* Done::process(void* data)
{
	State* returnState = this;
	RecipeData* recipeData = (RecipeData*) data;

	// if the A button pressed or 60 second time out go back to WELCOME
	if (recipeData->keypad->getKey() == 10 ||
		(getTick() - startTick) > 60000)
	{
		// display welcome message code goes here
		std::cout << WELCOME_MESSAGE;
		recipeData->display->write(WELCOME_MESSAGE, 150);
		returnState = this->stateManager->getState(WELCOME);
	}
	
	return returnState;
}

void Done::setup(void* data)
{
	startTick = getTick();
}
