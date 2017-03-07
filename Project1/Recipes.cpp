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
    
    recipe = new Recipe();
    recipe->setName("Easy Jambalaya");
    recipe->addIngredientString("1 tbsp oil\n1 lb boneless chicken breasts, cut\n1/2 lb smoked sausage\n1 medium onion\n1tbsp McCormick Cajun seasoning\n2 1/2 cups chicken broth\n1 can stewed tomatoes\1 cup long grain rice");
    //My lack of cooking knowledge renders me useless when adding the measured part.
    /*recipe->addMeasured(new Measured("rice", 224));
    recipe->addMeasured(new Measured("Water", 448));*/
    recipe->addStep("Heat oil in large heavy skillet on medium-high heat. Add chicken and sausage; cook and stir 5 minutes. Remove from skillet.");
    recipe->addStep("Stir onion into skillet; cook and stir on medium heat 2 minutes or until onion is softened. Stir in Seasoning.");
    recipe->addStep("Stir in broth and tomatoes; bring to boil. Stir in rice. Reduce heat to low; cover and cook 20 to 25 minutes or until rice is tender, stirring occasionally.");    
    this->mRecipeList.push_back(recipe);

    recipe = new Recipe();
    recipe->setName("Broccoli & Turkey Pasta");
    recipe->addIngredientString("3/4lb orecchiette\n2cups broccoli florets\n3tbsp olive oil\n1lb ground turkey\n2 cloves garlic\n1tsp fennel seed\n1/2tsp crushed red pepper\nkosher salt\nParmesan");
    /*recipe->addMeasured(new Measured("Beans", 224));
    recipe->addMeasured(new Measured("Water", 448));*/
    recipe->addStep("Cook the pasta according to the respective package directions. Add broccoli during hte last minute. Drain and return the pasta and broccoli to the pot.");
    recipe->addStep("Heat 1tbsp of the oil in a large skillet over medium-high heat. Add the turkey, garlic, fennel seed, and red pepper and cook; breaking up the meat with a spoon, until browned, 3 to 5 minutes; season with 1/2tsp salt.");
    recipe->addStep("Toss the turkey mixture with the pasta and broccoli and the remaining 2 tablespoons of oil. Serve with parmesan.");   
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
