#include "../common/State.h"

struct RecipeData
{
	Display* display;
	KeyPad* keypad;
	Scale* scale;
	CurrentRecipe* currentRecipe;
};

class Initial : public State
{
	public:
		Initial(RecipeData* data);
};

class Welcome : public State
{
	public:
		Welcome(RecipeData* data);
};

class RecipeList : public State
{
	public:
		RecipeList(RecipeData* data);
};

class DisplayIngredients : public State
{
	public:
		DisplayIngredients(RecipeData* data);
};

class ZeroScale : public State
{
	public:
		ZeroScale(RecipeData* data);
};

class Fill : public State
{
	public:
		Fill(RecipeData* data);
};

class Validate : public State
{
	public:
		Validate(RecipeData* data);
};

class ErrorFill : public State
{
	public:
		ErrorFile(RecipeData* data);
};

class AdditionalStep : public State
{
	public:
		AdditionalStep(RecipeData* data);
};

class Done : public State
{
	public:
		Done(RecipeData* data);
}