#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include "../common/Peripherals.h"
#include <sys/unistd.h>
#include <bitset>
#include <bitset>
#include <string>

using namespace std;

class Display 
{
public:
	Display();
	Display(int RegisterSelect, int ReadWrite, int CLK_1, int CLK_2, int DB7, int DB6, int DB5, int DB4, int DB3, int DB2, int DB1, int DB0);
	
	void setup();
	void initialize();
	void write(string Text);
	void clear();
	void reset();

private:
	GPIOPin* RegisterSelect;
	GPIOPin* ReadWrite;
	GPIOPin* CLK_1 = gpio->getPin(E1);
	GPIOPin* CLK_2 = gpio->getPin(E2);
	GPIOPin* DataBus[8] = { gpio->getPin(DB0), gpio->getPin(DB1), gpio->getPin(DB2), gpio->getPin(DB3), gpio->getPin(DB4), gpio->getPin(DB5), gpio->getPin(DB6), gpio->getPin(DB7) };

	void reset(int i);
	void clear(int i);
	
	void operate(int Mode, bitset<8> Instruction, long Duration, int Clock);
	void loadDataBus(bitset<8> Instruction);
	void enable(int i);
	void setMode(int MODE);
	void getMode(int MODE);
	void writeData(bitset<8> Data);
	void checkBusy(int Clock, int PreviousMode);
	void switchActiveClock();
	
	PeripheralFactory pf;
	GPIOSystem* gpio = (GPIOSystem*) pf.getPeripheral(PERIPHERAL_GPIO);
	
	bitset<8> getDDRAMAddress();
	void setDDRAMAddress(bitset<8> Address);
	void setCGRAMAddress(bitset<8> Address);

	// Trackers
	int CURRENT_MODE;
	int DDRAM_ADDRESS;
	int CGRAM_ADDRESS;
	int ACTIVE_CLOCK = 1;
	
	
	char characterMap[128]= {
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', '!', '"', '#', '$', '%', '&',  39, '(', ')', '*', '+', ',', '_', '.', '/',
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?',
		'@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
		'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', 165, ']', '^', '_',
		 96, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
		'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', ' ', ' '
	};
	
// GPIO Pins
 int DB7 =8;	// Data bus
int DB6 =15;
int DB5	= 		7;
int DB4	= 		18;
int DB3		= 	1;
int DB2			= 23;
int DB1			= 12;
int DB0			= 24;

int RW	= 		25;		// Read/!Write
int RS	= 		20;		// Register Select
int E1	= 		16;		// Top half of the display enabler
int E2	= 		21;		// Bottom half of the display enabler

// Operation Modes
int LL	= 		0;
int LH	= 		1;
int HL	= 		2;
int HH	= 		3;

// Top or bottom of the screen
int TOP	= 		1;
int BOTTOM = 		2;

// Instruction Set
int CLEAR= 		0x01;
int HOME= 		0x02;
int ENTRY_MODE= 	0x06;	// Shift cursor to the right
int OFF		= 	0x08;	// Display, cursor, and blink all off
int ON		= 	0x0F;	// Display, cursor, and blink all on
int SHIFT	= 	0x14;	// Shift cursor to the right
int FN_SET	= 	0x3C;	// 8-bit mode, 2-line mode, 5x11 dots

// Temp values
int TEST= 		2;

const long TIME_1 = 0.00041;
	const long TIME_2 = 0.003;

};

#endif // DISPLAY_H
