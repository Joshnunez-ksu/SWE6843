#include <iostream>
#include "../common/Peripherals.h"
#include <sys/unistd.h>
#include <bitset>
#include <string>
using namespace std;

Display Display::Display()
{
	PeripheralFactory pf;
	GPIOSystem* gpio = (GPIOSystem*) pf.getPeripheral(PERIPHERAL_GPIO);
}

Display Display::Display(int RegisterSelect, int ReadWrite, int CLK_1, int CLK_2, int DB7, int DB6, int DB5, int DB4, int DB3, int DB2, int DB1, int DB0)
{
	PeripheralFactory pf;
	GPIOSystem* gpio = (GPIOSystem*) pf.getPeripheral(PERIPHERAL_GPIO);
	
	this->RegisterSelect = gpio->getPin(RS);
	this->ReadWrite = gpio->getPin(ReadWrite);
	this->CLK_1 = gpio->getPin(E1);
	this->CLK_2 = gpio->getPin(E2);
	DataBus[8] = { gpio->getPin(DB0), gpio->getPin(DB1),
							gpio->getPin(DB2), gpio->getPin(DB3),
							gpio->getPin(DB4), gpio->getPin(DB5) ,
							gpio->getPin(DB6), gpio->getPin(DB7) };
	
						
	setup();
	sleep(TEST);
	initialize();
	sleep(TEST);
	setDDRAMAddress(0x80);
	
}

void Display::setup()
{
	RegisterSelect->setDirection(OUT);
	RegisterSelect->setValue(LOW);
	
	ReadWrite->setDirection(OUT);
	ReadWrite->setValue(LOW);
	
	CLK_1->setDirection(OUT);		// Both halves of the display
	CLK_1->setValue(LOW);
	CLK_2->setDirection(OUT);
	CLK_2->setValue(LOW);
	
	for (int i = 0; i < 8; i++)		// Data bus
	{
		DataBus[i]->setDirection(OUT);
		DataBus[i]->setValue(LOW);
		}
}

void Display::initialize()
{
	sleep(0.1);
	
	operate(LL, FN_SET, TIME_1, TOP);
	operate(LL, FN_SET, TIME_1, BOTTOM);
	operate(LL, FN_SET, TIME_1, TOP);
	operate(LL, FN_SET, TIME_1, BOTTOM);
	operate(LL, FN_SET, TIME_1, TOP);
	operate(LL, FN_SET, TIME_1, BOTTOM);
	operate(LL, FN_SET, 0.000053, BOTTOM);
	
	operate(LL, OFF, TIME_1, TOP);
	operate(LL, OFF, TIME_1, BOTTOM);
	
	operate(LL, CLEAR, TIME_2, TOP);
	operate(LL, CLEAR, TIME_2, BOTTOM);
	
	operate(LL, ENTRY_MODE, 0.000053, TOP);
	operate(LL, ENTRY_MODE, 0.000053, BOTTOM);
	enable(2);
	sleep(TIME_1);
	
	operate(LL, ON, TIME_1, TOP);
}

void Display::write(string text)
{
	clear();
	
	for(size_t n = 0; n < text.size(); n++)
	{
		for(int i = 32; i < 128; i++)
		{
			if(text[n] == 32)
				writeData(0x20);
			else if( text[n] == characterMap[i] )
				{
					bitset<8> temp (i);
					writeData(temp);
				}
		}
	}
}

void Display::reset()
{
	reset(1);	reset(2);
}

void Display::reset(int i)
{
	clear(1);
	clear(2);
	operate(LL, FN_SET, TIME_1, TOP);	
	operate(LL, FN_SET, TIME_1, BOTTOM);	// Function set
	operate(LL, 0x08, TIME_1, TOP);
	operate(LL, 0x08, TIME_1, BOTTOM);		// Turn display off
	operate(LL, 0x07, TIME_1, TOP);
	operate(LL, 0x07, TIME_1, BOTTOM);		// No shift
}

void Display::clear()
{
	clear(1);	clear(2);
}

void Display::clear(int Clock)
{
	operate(LL, CLEAR, TIME_2, Clock);
	setDDRAMAddress(0x00);
}


void Display::operate(int Mode, bitset<8> Instruction, long Duration, int Clock)
{
	// Register Select, Read/!Write
	setMode(Mode);
	
	// Load instruction into data bus
	loadDataBus(Instruction);
	enable(Clock);
	sleep(Duration);
	checkBusy(Clock, Mode);
}

void Display::loadDataBus(bitset<8> Instruction>)
{
	for ( int i = 0; i < 8; i++ )
	{
		if ( Instruction[i] == 0 )			DataBus[i]->setValue(LOW);
		else if ( Instruction[i] == 1 )	DataBus[i]->setValue(HIGH);
	}
}

void setCGRAMAddress(bitset<8> address)
{
	address = address |= 0x40;
	
	operate(LL, address, 0.000037, ACTIVE_CLOCK);
	cout << "CGRAM set" << endl;
}

void setDDRAMAddress(bitset<8> address)
{
	DDRAM_ADDRESS = address;
	operate(LL, DDRAM_ADDRESS, 0.000037, ACTIVE_CLOCK);
}

bitset<8> getDDRAMAddress()
{
	return DDRAM_ADDRESS;
}

void Display::enable(int i)
{
	DDRAM_ADDRESS = address;
	operate(LL, DDRAM_ADDRESS, 0.000037, ACTIVE_CLOCK);
}

void Display::getMode(int MODE)
{
	
}

void Display::setMode(int MODE)
{
	VOLTAGE RS_MODE;
	VOLTAGE RW_MODE;
	
	switch(MODE)
	{
		case LL:
		{
			RS_MODE = LOW;
			RW_MODE = LOW;
		}	break;
		case LH:
		{
			RS_MODE = LOW;
			RW_MODE = HIGH;
		}	break;
		case HL:
		{
			RS_MODE = HIGH;
			RW_MODE = LOW;
		}	break;
		case HH:
		{
			RS_MODE = LOW;
			RW_MODE = LOW;
		}	break;
	}

	RegisterSelect->setValue(RS_MODE);
	ReadWrite->setValue(RW_MODE);
}

void Display::writeData(bitset<8> Data)
{
	operate(HL, data, 0.000037, ACTIVE_CLOCK);
	
	if (DDRAM_ADDRESS == 0x27) {
		DDRAM_ADDRESS = 0x40;
			cout << "DDRAM_ADDRESS = " << DDRAM_ADDRESS << endl;;
	} else if (DDRAM_ADDRESS == 0x67) {
		DDRAM_ADDRESS = 0x00;
		cout << "DDRAM_ADDRESS = " << DDRAM_ADDRESS << endl;;
		switchActiveClock();
	} else 		{
		int temp = DDRAM_ADDRESS.to_ulong();
			temp++;
			DDRAM_ADDRESS = bitset<8> (temp);
			cout << "DDRAM_ADDRESS = " << DDRAM_ADDRESS << endl;;
	} 
}

void Display::checkBusy(int Clock, int PreviousMode)
{
	setMode(LH);
	loadDataBus(0x00);
	enable(Clock);
	while (DataBus[7]->getValue() == HIGH)
	{
		sleep(0.000001);
	}
	
	setMode(PreviousMode);
}

void Display::switchActiveClock()
{
	setMode(LH);
	loadDataBus(0x00);
	enable(Clock);
	while (DataBus[7]->getValue() == HIGH)
	{
		sleep(0.000001);
	}
	
	setMode(PreviousMode);
}

