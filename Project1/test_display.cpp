#include <iostream>
#include "../common/Peripherals.h"
#include <sys/unistd.h>
#include <bitset>

// GPIO Pins
#define DB7			8		// Data bus
#define DB6			15
#define DB5			7
#define DB4			18
#define DB3			1
#define DB2			23
#define DB1			12
#define DB0			24

#define RW			25		// Read/!Write
#define RS			20		// Register Select
#define E1			16		// Top half of the display enabler
#define E2			21		// Bottom half of the display enabler

// Operation Modes
#define LL			0
#define LH			1
#define HL			2
#define HH			3

// Top or bottom of the screen
#define TOP			1
#define BOTTOM		2

// Instruction Set
#define CLEAR		0x01
#define HOME		0x02
#define ENTRY_MODE	0x06	// Shift cursor to the right
#define OFF			0x08	// Display, cursor, and blink all off
#define ON			0x0F	// Display, cursor, and blink all on
#define SHIFT		0x14	// Shift cursor to the right
#define FN_SET		0x3C	// 8-bit mode, 2-line mode, 5x11 dots

// Temp values
#define TEST		2

using namespace std;

class Display
{
public:
	PeripheralFactory pf;
	GPIOSystem* gpio = (GPIOSystem*) pf.getPeripheral(PERIPHERAL_GPIO);
	
	GPIOPin* RegisterSelect = gpio->getPin(RS);
	GPIOPin* Read_Write = gpio->getPin(RW);
	GPIOPin* CLK_1 = gpio->getPin(E1);
	GPIOPin* CLK_2 = gpio->getPin(E2);
	GPIOPin* DataBus[8] = { gpio->getPin(DB0), gpio->getPin(DB1),
							gpio->getPin(DB2), gpio->getPin(DB3),
							gpio->getPin(DB4), gpio->getPin(DB5) ,
							gpio->getPin(DB6), gpio->getPin(DB7) };
	
	const long TIME_1 = 0.00041;
	const long TIME_2 = 0.003;
		
	int CURRENT_MODE;
	int ACTIVE_CLOCK = TOP;
	bitset<8> DDRAM_ADDRESS = 0x00;
	
	// h
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
	
	void setup() // h
	{
		RegisterSelect->setDirection(OUT);
		RegisterSelect->setValue(LOW);
		
		Read_Write->setDirection(OUT);
		Read_Write->setValue(LOW);
		
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
	
	void initialize() // h
	{
		sleep(0.1);
		
		operate(LL, FN_SET, TIME_1, TOP);
		operate(LL, FN_SET, TIME_1, BOTTOM);
		operate(LL, FN_SET, TIME_1, TOP);
		operate(LL, FN_SET, TIME_1, BOTTOM);
		operate(LL, FN_SET, TIME_1, TOP);
		operate(LL, FN_SET, TIME_1, BOTTOM);
		operate(LL, FN_SET, 0.000053, BOTTOM);
			//~ cout << endl<< "function set" << endl;
			//~ sleep(TEST);
		
		operate(LL, OFF, TIME_1, TOP);
		operate(LL, OFF, TIME_1, BOTTOM);
			//~ cout << endl<< "display off" << endl;
			//~ sleep(TEST);
			
		operate(LL, CLEAR, TIME_2, TOP);
		operate(LL, CLEAR, TIME_2, BOTTOM);
			cout << endl << "display cleared" << endl;
			//~ sleep(TEST);
		
		operate(LL, ENTRY_MODE, 0.000053, TOP);
		operate(LL, ENTRY_MODE, 0.000053, BOTTOM);
		enable(2); sleep(TIME_1);
			//~ cout << endl<< "entry mode set" << endl;
			//~ sleep(TEST);
			
		operate(LL, ON, TIME_1, TOP);
			//~ cout << endl << "display on" << endl;
			//~ sleep(TEST);
	}
	
	void reset() // h
	{
		operate(LL, CLEAR, TIME_2, TOP);
		operate(LL, CLEAR, TIME_2, BOTTOM);		// Clear display
		operate(LL, FN_SET, TIME_1, TOP);	
		operate(LL, FN_SET, TIME_1, BOTTOM);		// Function set
		operate(LL, 0x08, TIME_1, TOP);
		operate(LL, 0x08, TIME_1, BOTTOM);		// Turn display off
		operate(LL, 0x07, TIME_1, TOP);
		operate(LL, 0x07, TIME_1, BOTTOM);		// No shift
		
		cout << "display has been reset" << endl;
	}
	
	void setCGRAMAddress(bitset<8> address) // h
	{
		address = address |= 0x40;
		
		operate(LL, address, 0.000037, ACTIVE_CLOCK);
		cout << "CGRAM set" << endl;
	}
	
	void setDDRAMAddress(bitset<8> address) // h
	{		
		DDRAM_ADDRESS = address;
		operate(LL, DDRAM_ADDRESS, 0.000037, ACTIVE_CLOCK);
		//~ cout << "DDRAM set" << endl;
	}
	
	bitset<8> getDDRAMAddress() // h
	{
		return DDRAM_ADDRESS;
	}
	
	void write(string text) // h
	{
		clear(1);
		
		for(size_t n = 0; n < text.size(); n++)
		{
			for(int i = 32; i < 128; i++)
			{
				if(text[n] == 32)
					write_data(0x20);
				else if( text[n] == characterMap[i] )
					{
						bitset<8> temp (i);
						write_data(temp);
					}
			}
			//~ sleep(TEST);
		}	
	}
	void operate(int Mode, bitset<8> Instruction, long Duration, int Clock) // h
	{
		
		// Register Select, Read/!Write
		setMode(Mode);
		
		// Load instruction into data bus
		loadDB(Instruction);
		enable(Clock);
		sleep(Duration);
		checkBusy(Clock, Mode);
	}

	void clear(int Clock) // h
	{
		operate(LL, CLEAR, TIME_2, Clock);
		setDDRAMAddress(0x00);
	}
private:
	
	void write_data(bitset<8> data) // h
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
	
	
	void setMode(int MODE) / h
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
		Read_Write->setValue(RW_MODE);
	}
	
	void checkMode(int CurrentMode) // h
	{
		
	}
	
	void loadDB(bitset<8> Instruction) // h
	{
		for ( int i = 0; i < 8; i++ )
		{
			if ( Instruction[i] == 0 )			DataBus[i]->setValue(LOW);
			else if ( Instruction[i] == 1 )	DataBus[i]->setValue(HIGH);
		}
	}
	
	void enable(int i) // h
	{
		if (i == 1)
		{
			CLK_1->setValue(HIGH);
			sleep(0.0001);
			CLK_1->setValue(LOW);
		} else if (i == 2)
		{
			CLK_2->setValue(HIGH);
			sleep(0.0001);
			CLK_2->setValue(LOW);
		}
	}
	
	void checkBusy(int Clock, int PreviousMode) // h
	{
		setMode(LH);
		loadDB(0x00);
		enable(Clock);
		while (DataBus[7]->getValue() == HIGH)
		{
			sleep(0.000001);
		}
		
		setMode(PreviousMode);
	}
	
	void switchActiveClock() // h 
	{
		if (ACTIVE_CLOCK == TOP) {
			ACTIVE_CLOCK = BOTTOM;
			cout << "Now using bottom screen" << endl;
		} else if (ACTIVE_CLOCK == BOTTOM) {
			ACTIVE_CLOCK == TOP;
			cout << "Now using top screen" << endl;
		}
	}

};

int main(int argc, char **argv)
{
	Display LCD;
	LCD.setup();
	sleep(TEST);
	LCD.initialize();
	sleep(TEST);
	
	LCD.setDDRAMAddress(0X80);
	//~ sleep(TEST);
	
	LCD.write("HelloWorld!");
	sleep(4);
	//~ LCD.clear(TOP);
	
	int i = 0;
	string text;
	while(i < 10)
	{
		cin >> text;
		LCD.write(text);
	}

	//~ sleep(TEST*5);
	//~ LCD.reset();
	return 0;
}
