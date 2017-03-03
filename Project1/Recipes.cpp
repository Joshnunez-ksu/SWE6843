/********************************************************************************
********************************************************************************/
#include "Recipes.h"
#include <cstring>

Recipes::Recipes()
{
    Recipe* recipe = new Recipe();
    recipe->setName("Rice");
    recipe->addIngredientString("1 Cup Rice\n2 Cups Water");
    recipe->addMeasured(new Measured("Rice", 224));
    recipe->addMeasured(new Measured("Water", 448));
    recipe->addStep("Combine Rice and Water in pot and place pot on stove.");
    recipe->addStep("Heat water on high until it boils.  Then reduce heat to low and cover the pot for 20 minutes.");
    this->mRecipeList.push_back(recipe);
    
    recipe = new Recipe();
    recipe->setName("Beans");
    recipe->addIngredientString("1 Cup beans\n4 Cups Water");
    recipe->addMeasured(new Measured("Beans", 224));
    recipe->addMeasured(new Measured("Water", 448));
    recipe->addStep("Combine Beans and Water in pot and place pot on stove.");
    recipe->addStep("Heat water on high until it boils.  Then reduce heat to low and cover the pot for 20 minutes.");    
    this->mRecipeList.push_back(recipe);
}

Recipes::~Recipes()
{
}

Recipe* Recipes::getRecipe(int index)
{
    Recipe* returnRecipe = (Recipe*) 0;
    if (index < this->mRecipeList.size())
    {
        returnRecipe = this->mRecipeList[index];
    }
    
    return returnRecipe;
}

Recipe::Recipe()
{
    this->mName = (char*) 0;
}

Recipe::~Recipe()
{
}

void Recipe::setName(const char* name)
{
    if (!this->mName)
    {
        this->mName = new char[20];
    }
    
    strcpy(this->mName, name);
}

char* Recipe::getName()
{
    return this->mName;
}

void Recipe::addMeasured(Measured* m)
{
    this->mMeasuredList.push_back(m);
}

Measured* Recipe::getMeasured(int index)
{
    Measured* returnMeasured = (Measured*) 0;
    
    if (index < this->mMeasuredList.size())
    {
        returnMeasured = this->mMeasuredList[index];
    }
    
    return returnMeasured;
}

void Recipe::addStep(const char* step)
{
    char* newStep = new char[160];
    strcpy(newStep, step);
    
    this->mStepList.push_back(newStep);
}

char* Recipe::getStep(int index)
{
    char* returnStep = (char*) 0;
    
    if (index < this->mStepList.size())
    {
        returnStep = this->mStepList[index];
    }
    
    return returnStep;
}

void Recipe::addIngredientString(const char* lines)
{
	char* newLine = new char[160];
	strcpy(newLine, lines);
	
	this->mIngredientLines.push_back(newLine);
}

char* Recipe::getIngredientString(int index)
{
	char* returnIngredient = (char*) 0;
	
	if (index < this->mIngredientLines.size())
	{
		returnIngredient = this->mIngredientLines[index];
	}
	
	return returnIngredient;
}
