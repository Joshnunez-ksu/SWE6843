#include "../common/State.h"
#include "Recipes.h"

#define		INITIAL				"StateInitial"
#define		WELCOME				"Welcome"
#define		RECIPELIST			"RecipeList"
#define		DISPLAYINGREDIENTS	"DisplayIngredients"
#define		ZEROSCALE			"ZeroScale"
#define		FILL				"Fill"
#define		VALIDATE			"Validate"
#define		ERRORFILL			"ErrorFill"
#define		ADDITIONALSTEP		"AdditionalStep"
#define		DONE				"Done"
      
      
class Display;

struct RecipeData
{
	Display*	display;
	KeyPad*		keypad;
	Scale*		scale;
	Recipes*	recipes;
	Recipe*		currentRecipe;
	Measured*	currentMeasured;
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
	private:
		int pageNumber;
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
		void setup(void* data){stepNumber = 0;};
		
	private:
		int stepNumber;
};

class Done : public State
{
	public:
		Done(StateManager* sm, PeripheralFactory* pf) : State(sm, pf) {};
		State* process(void* data);
		void setup(void* data);
	
	private:
		long startTick;
};
