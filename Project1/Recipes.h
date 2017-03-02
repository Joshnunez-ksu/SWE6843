/********************************************************************************
Recipe definitions
********************************************************************************/
#include <vector>

struct Measured
{
    Measured(const char* i, int g) : ingredient(i), grams(g){}
    const char*   ingredient;
    int     grams;
};

class Recipe
{
public:
    Recipe();
    ~Recipe();
    
    void setName(const char* name);
    void addMeasured(Measured* m);
    void addStep(const char* step);
    
    char*       getName();
    Measured*   getMeasured(int index);
    char*       getStep(int index);
    
private:
    std::vector<Measured*>   mMeasuredList;
    std::vector<char*>       mStepList;
    char*                    mName;
    

};

class Recipes
{
public:
    Recipes();
    ~Recipes();
    
    Recipe* getRecipe(int index);
    
private:
    std::vector<Recipe*> mRecipeList;

};
