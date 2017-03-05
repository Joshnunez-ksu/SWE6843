#include <iostream>
#include "Display.h"
//~ #include "../common/Peripherals.h"
#include "../common/Peripherals.cpp"
#include <sys/unistd.h>
#include <bitset>
#include <string>

using namespace std;

Display::Display(int RS, int RW, int E1, int E2, int DB7, int DB6, int DB5, int DB4, int DB3, int DB2, int DB1, int DB0)
{
	RegisterSelect = gpio->getPin(RS);
	ReadWrite = gpio->getPin(RW);
	CLK_1 = gpio->getPin(E1);
	CLK_2 = gpio->getPin(E2);
	
	DataBus[0] = gpio->getPin(DB0);
	DataBus[1] = gpio->getPin(DB1);
	DataBus[2] = gpio->getPin(DB2);
	DataBus[3] = gpio->getPin(DB3);
	DataBus[4] = gpio->getPin(DB4);
	DataBus[5] = gpio->getPin(DB5);
	DataBus[6] = gpio->getPin(DB6);
	DataBus[7] = gpio->getPin(DB7);
	
	sleep(0.5);
	
	// Setup
	RegisterSelect->setDirection(OUT);
	RegisterSelect->setValue(LOW);
	
	ReadWrite->setDirection(OUT);
	ReadWrite->setValue(LOW);
	
	CLK_1->setDirection(OUT);		// Both halves of the display
	CLK_1->setValue(LOW);
	CLK_2->setDirection(OUT);
	CLK_2->setValue(LOW);
	
	for (int i = 0; i < 8; i++)		{
		DataBus[i]->setDirection(OUT);
		DataBus[i]->setValue(LOW);
	}
	// End of setup
	
	// Initialzation
	operate(TOP, 0b00, 0x38, 0.000037);
	operate(BOTTOM, 0b00, 0x3C, 0.000037);
	operate(TOP, 0b00, 0x38, 0.000037);
	operate(BOTTOM, 0b00, 0x3C, 0.000037);
	operate(BOTTOM, 0b00, 0x3C, 0.000037);
	operate(TOP, 0b00, 0x38, 0.000053);
	operate(BOTTOM, 0b00, 0x3C, 0.000053);
	
	operate(TOP, 0b00, 0x08, 0.000037);
	operate(BOTTOM, 0b00, 0x08, 0.000037);
	
	operate(TOP, 0b00, 0x01, 0.00152);
	operate(BOTTOM, 0b00, 0x01, 0.00152);
	
	operate(TOP, 0b00, 0x06, 0.000037);
	operate(BOTTOM, 0b00, 0x06, 0.000037);
	
	operate(TOP, 0b00, 0x0C, 0.000037);
	operate(BOTTOM, 0b00, 0x0C, 0.000037);
	// End of initialization
	
	sleep(3);
	
	string text = "Recipe Wizard 3000";
	for(int i = 0; i < 40 + ((40 - text.size())/2); i++)
		operate(ACTIVE_CLOCK, 0b00, 0x14, 0.000037);
	//~ sleep(3);
	
	write(text);
	sleep(5);
	clear();
}

Display::~Display()
{
	
}

void Display::write(string text)
{
	//~ clear();
	
	for(size_t n = 0; n < text.size(); n++)
	{
		size_t i = 32;
		while((text[n] != characterMap[i]) && (i < 128))
			i++;
		
		if (i == 128) 	writeToDDRAM(ACTIVE_CLOCK, 0x20);		// Character not in character map; display a space
		else 			writeToDDRAM(ACTIVE_CLOCK, data (i));
	}
}

void Display::clear()
{
	operate(TOP, 0b00, 0x01, 0.00152);
	operate(BOTTOM, 0b00, 0x01, 0.00152);
	DDRAM_ADDRESS = 0x00;
}


// Internal Opeartions
void Display::operate(STATE Enabler, mode Mode, data Instruction, long Duration)
{
	// Register Select, Read/Write
	setMode(Mode);
	
	// Load data bus (formerly "loadDataBus(Instruction)"
	for ( int i = 0; i < 8; i++ )
	{
		if ( Instruction[i] == 0 )			DataBus[i]->setValue(LOW);
		else if ( Instruction[i] == 1 )	DataBus[i]->setValue(HIGH);
	}
	
	// Send instruction to half of display (formerly "enable(Enabler)")
	if (Enabler == TOP)
	{
		CLK_1->setValue(HIGH);
		sleep(0.0001);
		CLK_1->setValue(LOW);
	} else if (Enabler == BOTTOM)
	{
		CLK_2->setValue(HIGH);
		sleep(0.0001);
		CLK_2->setValue(LOW);
	}
	
	// Give the display time to execute operation
	sleep(Duration);
	
	// Won't take any operations if the display is still working
	checkBusy(Enabler);
}

void Display::setMode(mode Mode)
{
	if (Mode[1] == 0)			RegisterSelect->setValue(LOW);
	else if(Mode[1] == 1)		RegisterSelect->setValue(HIGH);
	
	if (Mode[0] == 0)			ReadWrite->setValue(LOW);
	else if(Mode[0] == 1)	ReadWrite->setValue(HIGH);
	
	CURRENT_MODE = Mode;
}

Display::mode Display::getMode()
{
	return CURRENT_MODE;
}

void Display::checkBusy(STATE Enabler)
{
	mode m = getMode();
	
	setMode(0b01);
	while (DataBus[7]->getValue() == HIGH)		// might can take out the comparison, since it's equivalent to 1/true
	{
		sleep(0.000001);
	}
	
	setMode(m);
}

void Display::writeToDDRAM(STATE Enabler, data Data)
{
	operate(ACTIVE_CLOCK, 0b10, Data, 0.000037);
	
	if (DDRAM_ADDRESS == 0x27) {
		DDRAM_ADDRESS = 0x40;
	} else if (DDRAM_ADDRESS == 0x67) {
		switchActiveClock();
		DDRAM_ADDRESS = 0x00;
	} else 		{
		int temp = DDRAM_ADDRESS.to_ulong();
		temp++;
		DDRAM_ADDRESS = data (temp);
	}
}

void Display::switchActiveClock()
{
	if (ACTIVE_CLOCK == TOP)			ACTIVE_CLOCK = BOTTOM;
	else if (ACTIVE_CLOCK == BOTTOM)	ACTIVE_CLOCK = TOP;
}


