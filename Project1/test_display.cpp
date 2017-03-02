#include <iostream>
#include "../common/Peripherals.h"
#include <bitset>
#include <sys/unistd.h>
#include <array>
#include <string>
#include <cassert>

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

// Top or bottom of the scree
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
	
	//~ GPIOPin* RegisterSelect = gpio->getPin(RS);
	//~ GPIOPin* Read_Write = gpio->getPin(RW);
	//~ GPIOPin* CLK_1 = gpio->getPin(E1);
	//~ GPIOPin* CLK_2 = gpio->getPin(E2);
	//~ GPIOPin* DataBus[8] = { gpio->getPin(DB0), gpio->getPin(DB1),
							//~ gpio->getPin(DB2), gpio->getPin(DB3),
							//~ gpio->getPin(DB4), gpio->getPin(DB5) ,
							//~ gpio->getPin(DB6), gpio->getPin(DB7) };
	
	const long TIME_1 = 0.00041;
	const long TIME_2 = 0.003;
	
	int CURRENT_MODE;
	int ACTIVE = 1;
	
	//~ char characterMap[128]= {
		//~ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		//~ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		//~ ' ', '!', '"', '#', '$', '%', '&',  39, '(', ')', '*', '+', ',', '_', '.', '/',
		//~ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?',
		//~ '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
		//~ 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', 165, ']', '^', '_',
		 //~ 96, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
		//~ 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', ' ', ' '
	//~ };
	
	void setup()
	{
		RegisterSelect->setDirection(OUT);
		RegisterSelect->setValue(LOW);
		
		Read_Write->setDirection(OUT);
		Read_Write->setValue(LOW);
		
		CLK_1->setDirection(OUT);		// Both halves of the display
		CLK_1->setValue(LOW);
		CLK_2->setDirection(OUT);
		CLK_2->setValue(LOW);
		
		//~ switch (POSITION)				// Control top or bottom half of display
		//~ {
			//~ case BOTH:
			//~ {
				//~ CLK_1->setDirection(OUT);		// Both halves of the display
				//~ CLK_1->setValue(LOW);
				//~ CLK_2->setDirection(OUT);
				//~ CLK_2->setValue(LOW);	break;
			//~ }
			//~ case TOP:
			//~ {
				//~ CLK_1->setDirection(OUT);		// Top half of the display
				//~ CLK_1->setValue(LOW);	break;
			//~ }
			//~ case BOTTOM:
			//~ {
				//~ CLK_2->setDirection(OUT);		// Bottom half of the display
				//~ CLK_2->setValue(LOW); break;
			//~ }
		//~ }

		for (int i = 0; i < 8; i++)		// Data bus
		{
			DataBus[i]->setDirection(OUT);
			DataBus[i]->setValue(LOW);
		}
	}
	
	void initialize()
	{
		sleep(0.1);
		
		operate(LL, 0x30, TIME_1, TOP);
		operate(LL, 0x30, TIME_1, BOTTOM);
		operate(LL, 0x30, 0.0001, TOP);
		operate(LL, 0x30, 0.0001, BOTTOM);
		operate(LL, 0x30, 0.0001, TOP);
		operate(LL, 0x30, 0.0001, BOTTOM);
		operate(LL, FN_SET, 0.000053, BOTTOM);
			cout << endl<< "function set" << endl;
			//~ sleep(TEST);
		
		operate(LL, OFF, TIME_1, TOP);
		operate(LL, OFF, TIME_1, BOTTOM);
			cout << endl<< "display off" << endl;
			//~ sleep(TEST);
			
		operate(LL, CLEAR, TIME_2, TOP);
		operate(LL, CLEAR, TIME_2, BOTTOM);
			cout << endl<< "display cleared" << endl;
			//~ sleep(TEST);
		
		operate(LL, ENTRY_MODE, 0.000053, TOP);
		operate(LL, ENTRY_MODE, 0.000053, BOTTOM);
		enable(2); sleep(TIME_1);
			cout << endl<< "entry mode set" << endl;
			//~ sleep(TEST);
			
		operate(LL, ON, TIME_1, TOP);
			cout << endl << "display on" << endl;
			//~ sleep(TEST);
	}
	
	void reset()
	{
		operate(LL, CLEAR, TIME_2, TOP);
		operate(LL, CLEAR, TIME_2, BOTTOM);		// Clear display
		operate(LL, 0x30, TIME_1, TOP);	
		operate(LL, 0x30, TIME_1, BOTTOM);		// Function set
		operate(LL, 0x08, TIME_1, TOP);
		operate(LL, 0x08, TIME_1, BOTTOM);		// Turn display off
		operate(LL, 0x07, TIME_1, TOP);
		operate(LL, 0x07, TIME_1, BOTTOM);		// No shift
		
		cout << "display has been reset" << endl;
	}
	
	void set_CGRAM(bitset<8> address)
	{
		address = address |= 0x40;
		
		operate(LL, address, 0.000037, ACTIVE);
		cout << "CGRAM set" << endl;
	}
	
	void set_DDRAM(bitset<8> address)
	{		
		operate(LL, address, 0.000037, ACTIVE);
		cout << "DDRAM set" << endl;
	}
	
	void write(string text)
	{
		for(size_t n = 0; n < text.size();  n++)
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
	void operate(int Mode, bitset<8> Instruction, long Duration, int Clock)
	{
		assert((Mode >= 0) & (Mode <= 4));
		
		// Register Select, Read/!Write
		setMode(Mode);
		
		// Load instruction into data bus
		loadDB(Instruction);
		enable(Clock);
		sleep(Duration);
		checkBusy(Clock, Mode);
	}

private:
	
	void write_data(bitset<8> data)
	{
		operate(HL, data, 0.000037, ACTIVE);
		//~ cout << "wrote some shit" << endl;
	}
	
	void setMode(int MODE)
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
	
	void checkMode(int CurrentMode)
	{
		
	}
	
	void loadDB(bitset<8> Instruction)
	{
		for ( int i = 0; i < 8; i++ )
		{
			if ( Instruction[i] == 0 )			DataBus[i]->setValue(LOW);
			else if ( Instruction[i] == 1 )	DataBus[i]->setValue(HIGH);
		}
	}
	
	void enable(int i)
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
	
	void checkBusy(int Clock, int PreviousMode)
	
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
};

int main(int argc, char **argv)
{
	Display LCD;
	LCD.setup();
	sleep(TEST);
	LCD.initialize();
	sleep(TEST);
	
	LCD.set_DDRAM(0X80);
	//~ sleep(TEST);
	
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
