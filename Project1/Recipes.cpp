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
    
    recipe = new Recipe();
    recipe->setName("Omelette au Fromage");
    recipe->addIngredientString("4 extra large eggs\nfine salt, to taste\nfresh groundb black pepper\n1 ounce parmesan cheese grated");
    recipe->addIngredientString("2 tablespoons butter\n2 ounces gruyere cheese, grated");
    recipe->addMeasured(new Measured("Parmesan cheese", 28));
    recipe->addMeasured(new Measured("Gruyere cheese", 56));
    recipe->addStep("Break the eggs into a bowl.");
    recipe->addStep("Season to taste with salt, pepper, and nutmeg.");
    recipe->addStep("Add Parmeasan cheese.");
    recipe->addStep("Beat the eggs with a fork until just mixed.");
    recipe->addStep("Heat a large frying pan over medium heat.");
    recipe->addStep("Add the butter and allow it to melt and color slightly.");
    recipe->addStep("Add the beaten eggs.");
    recipe->addStep("Use the flat side of the fork to stir the eggs until start toset.");
    recipe->addStep("Smooth the top and leave the eggs to cook.");
    recipe->addStep("Add the Gruyere cheese over the surface.");
    recipe->addStep("Fold the omelette into thirds and flip it onto a heated servimg plate.");
    
    
    recipe = new Recipe();
    recipe->setName("Black Beans and Rice");
    recipe->addIngredientString("2 cloves garlic, minced\n3/4 cup uncooked white rice\n1 1/2 cups low sodium\n1 teaspoon ground cumin");
    recipe->addIngredientString("1/4 teaspon cayenne
    
    
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
