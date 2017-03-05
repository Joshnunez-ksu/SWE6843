#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include "../common/Peripherals.h"
#include <sys/unistd.h>
#include <bitset>
#include <string>

using namespace std;

class Display 
{
public:
	typedef bitset<2> mode;
	typedef bitset<8> data;
	enum STATE {
		TOP = 1, BOTTOM = 2
	};
	
	Display(int RegisterSelect, int ReadWrite, int CLK_1, int CLK_2, int DB7, int DB6, int DB5, int DB4, int DB3, int DB2, int DB1, int DB0);
	~Display();
	
	void write(string Text);
	void clear();
	
private:
	PeripheralFactory pf;
	GPIOSystem*	gpio = (GPIOSystem*) pf.getPeripheral(PERIPHERAL_GPIO);
	GPIOPin*	RegisterSelect;
	GPIOPin*	ReadWrite;
	GPIOPin*	CLK_1;
	GPIOPin*	CLK_2;
	GPIOPin*	DataBus[8];
	
	char	characterMap[128]= {
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',			// 0 - 15
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',			// 16 - 31
		' ', '!', '"', '#', '$', '%', '&',  39, '(', ')', '*', '+', ',', '-', '.', '/',			// 32 - 47
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?',			// 48 - 63
		'@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',			// 64 - 79
		'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', 165, ']', '^', 95,			// 80 - 05
		 96, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',			// 96 - 111
		'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', ' ', ' '			// 112 - 127
	};
	
	
	// Internal Operations
	void	operate(STATE Enabler, mode Mode, data Instruction, long Duration);
	void	setMode(mode);
	mode	getMode();
	void	checkBusy(STATE Enabler);
	void	writeToDDRAM(STATE Enabler, data Data);
	void	switchActiveClock();
	
	// Variables
	mode	CURRENT_MODE;
	STATE	ACTIVE_CLOCK = TOP;
	data	DDRAM_ADDRESS = 0x00;
	data	CGRAM_ADDRESS = 0x00;
};

#endif // DISPLAY_H
