#include "../common/State.h"
#include "Recipes.h"

class Display;

struct RecipeData
{
	Display* display;
	KeyPad* keypad;
	Scale* scale;
	Recipes* recipes;
	int currentRecipe;
};

class Initial : public State
{
	public:
		Initial(StateManager* sm, PeripheralFactory* pf) : State(sm, pf) {};
		State* process(void* data);
		void setup(void* data){};
};

class Welcome : public State
{
	public:
		Welcome(StateManager* sm, PeripheralFactory* pf) : State(sm, pf) {};
		State* process(void* data);
		void setup(void* data){};
};

class RecipeList : public State
{
	public:
		RecipeList(StateManager* sm, PeripheralFactory* pf) : State(sm, pf) {};
		State* process(void* data);
		void setup(void* data){};
};

class DisplayIngredients : public State
{
	public:
		DisplayIngredients(StateManager* sm, PeripheralFactory* pf) : State(sm, pf) {};
		State* process(void* data);
		void setup(void* data){};
};

class ZeroScale : public State
{
	public:
		ZeroScale(StateManager* sm, PeripheralFactory* pf) : State(sm, pf) {};
		State* process(void* data);
		void setup(void* data){};
};

class Fill : public State
{
	public:
		Fill(StateManager* sm, PeripheralFactory* pf) : State(sm, pf) {};
		State* process(void* data);
		void setup(void* data){};
};

class Validate : public State
{
	public:
		Validate(StateManager* sm, PeripheralFactory* pf) : State(sm, pf) {};
		State* process(void* data);
		void setup(void* data){};
};

class ErrorFill : public State
{
	public:
		ErrorFill(StateManager* sm, PeripheralFactory* pf) : State(sm, pf) {};
		State* process(void* data);
		void setup(void* data){};
};

class AdditionalStep : public State
{
	public:
		AdditionalStep(StateManager* sm, PeripheralFactory* pf) : State(sm, pf) {};
		State* process(void* data);
		void setup(void* data){};
};

class Done : public State
{
	public:
		Done(StateManager* sm, PeripheralFactory* pf) : State(sm, pf) {};
		State* process(void* data);
		void setup(void* data){};
};
