/********************************************************************************
key pad test
********************************************************************************/

#include <iostream>
#include "../common/Peripherals.h"

/*
class KeyPad
{
public:
	KeyPad(GPIOPin* pin0, GPIOPin* pin1, GPIOPin* pin2, GPIOPin* pin3, GPIOPin* pin4, GPIOPin* pin5, GPIOPin* pin6, GPIOPin* pin7);
	~KeyPad();

	int getKey();

private:
	GPIOPin*	pins[8];
	
	char		buffer[4];
	int			padGrid[4][4];

};

KeyPad::KeyPad(GPIOPin* pin0, GPIOPin* pin1, GPIOPin* pin2, GPIOPin* pin3, GPIOPin* pin4, GPIOPin* pin5, GPIOPin* pin6, GPIOPin* pin7)
{
	this->pins[0] = pin0;
	this->pins[1] = pin1;
	this->pins[2] = pin2;
	this->pins[3] = pin3;
	this->pins[4] = pin4;
	this->pins[5] = pin5;
	this->pins[6] = pin6;
	this->pins[7] = pin7;
	
	for (int i=0; i < 4; i++)
	{
		// set the column pins to OUT so we can send data
		this->pins[i]->setDirection(OUT);
		this->pins[i]->setValue(LOW);
		
		this->buffer[i] = 0;
	}
	
	for (int i=4; i < 8; i++)
	{
		this->pins[i]->setDirection(IN);
	}
	
	this->padGrid[0][0] = 1;
	this->padGrid[0][1] = 2;
	this->padGrid[0][2] = 3;
	this->padGrid[0][3] = 10;
	this->padGrid[1][0] = 4;
	this->padGrid[1][1] = 5;
	this->padGrid[1][2] = 6;
	this->padGrid[1][3] = 11;
	this->padGrid[2][0] = 7;
	this->padGrid[2][1] = 8;
	this->padGrid[2][2] = 9;
	this->padGrid[2][3] = 12;
	this->padGrid[3][0] = 14;
	this->padGrid[3][1] = 0;
	this->padGrid[3][2] = 15;
	this->padGrid[3][3] = 13;
	
}

KeyPad::~KeyPad()
{
}

int KeyPad::getKey()
{
	int returnKey = -1;
	
	for(int i=0; i < 4; i++)
	{
		this->pins[i]->setValue(HIGH);
		
		for(int j=4; j < 8; j++)
		{
			this->buffer[j-4] = (this->buffer[j-4] << 1) | (char)this->pins[j]->getValue() | 0xe0;
			
			if (this->buffer[j-4] == 0xf0)
			{
				this->buffer[j-4] = 0xe0;
				
				return this->padGrid[i][j-4];
			}
		}
		
		this->pins[i]->setValue(LOW);
	}

	nanowait(0, 1000000);

	return returnKey;
}
*/
int main()
{
	PeripheralFactory pf;
	GPIOSystem* gpio = (GPIOSystem*) pf.getPeripheral(PERIPHERAL_GPIO);

	KeyPad keypad(gpio->getPin(17),
					gpio->getPin(27),
					gpio->getPin(22),
					gpio->getPin(5),
					gpio->getPin(6),
					gpio->getPin(13),
					gpio->getPin(19),
					gpio->getPin(26));
	
	int keyCount = 0;
	int keyPressed = -1;
	
	while (keyCount < 16)
	{
		keyPressed = keypad.getKey();
		
		if (keyPressed != -1)
		{
			std::cout << "Key : " << keyPressed << "\n";
			keyCount++;
		}
		
	}
	
	return 0;
};
