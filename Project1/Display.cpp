#include <iostream>
#include "Display.h"
#include "../common/Peripherals.h"
#include <sys/unistd.h>

using namespace std;

Display::Display(GPIOPin* RegisterSelect, GPIOPin* ReadWrite, GPIOPin* CLK_1, GPIOPin* CLK_2, GPIOPin* DB7, GPIOPin* DB6, GPIOPin* DB5, GPIOPin* DB4, GPIOPin* DB3, GPIOPin* DB2, GPIOPin* DB1, GPIOPin* DB0)
{
	// Setup
	this->RegisterSelect = RegisterSelect;
	this->ReadWrite = ReadWrite;
	this->CLK_1 = CLK_1;
	this->CLK_2 = CLK_2;
	
	DataBus[0] = DB0;
	DataBus[1] = DB1;
	DataBus[2] = DB2;
	DataBus[3] = DB3;
	DataBus[4] = DB4;
	DataBus[5] = DB5;
	DataBus[6] = DB6;
	DataBus[7] = DB7;
	
	RegisterSelect->setDirection(OUT);
	RegisterSelect->setValue(LOW);
	
	ReadWrite->setDirection(OUT);
	ReadWrite->setValue(LOW);
	
	CLK_1->setDirection(OUT);
	CLK_1->setValue(LOW);
	CLK_2->setDirection(OUT);
	CLK_2->setValue(LOW);
	
	for (int i = 0; i < 8; i++)		{
		DataBus[i]->setDirection(OUT);
		DataBus[i]->setValue(LOW);
	}
	// End of Setup
	
	// Initialzation
	operate(TOP, 0b00, 0x38, 0.000037);
	operate(BOTTOM, 0b00, 0x3C, 0.000037);
	operate(TOP, 0b00, 0x38, 0.000037);
	operate(BOTTOM, 0b00, 0x3C, 0.000037);
	operate(BOTTOM, 0b00, 0x3C, 0.000037);
	operate(TOP, 0b00, 0x38, 0.000053);
	operate(BOTTOM, 0b00, 0x3C, 0.000053);			// Set function
	
	operate(TOP, 0b00, 0x08, 0.000037);
	operate(BOTTOM, 0b00, 0x08, 0.000037);			// Turn display off
	
	operate(TOP, 0b00, 0x01, 0.00152);
	operate(BOTTOM, 0b00, 0x01, 0.00152);			// Clear display
	
	operate(TOP, 0b00, 0x06, 0.000037);
	operate(BOTTOM, 0b00, 0x06, 0.000037);			// Set entry mode
	
	operate(TOP, 0b00, 0x0C, 0.000037);
	operate(BOTTOM, 0b00, 0x0C, 0.000037);			// Turn display on
	// End of Initialization
	
	sleep(1);
	
	string text = "Recipe Wizard 3000";
	for(int i = 0; i < 40 + ((40 - text.size())/2); i++)
		operate(ACTIVE_CLOCK, 0b00, 0x14, 0.000037);
	
	write(text);
	sleep(5);
	clear();
}

Display::~Display()
{
	
}

void Display::write(string text)
{
	clear();
	
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
	operate(TOP, 0b00, 0x08, 0.000037);
	operate(BOTTOM, 0b00, 0x08, 0.000037);			// Turn display off
	
	operate(TOP, 0b00, 0x01, 0.00152);
	operate(BOTTOM, 0b00, 0x01, 0.00152);			// Clear display
	
	operate(TOP, 0b00, 0x0C, 0.000037);
	operate(TOP, 0b00, 0x0C, 0.000037);				// Turn display on
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
