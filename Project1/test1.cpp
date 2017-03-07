/********************************************************************************
Test Key pad with Display
********************************************************************************/

#include "../common/Peripherals.h"

int main()
{
	PeripheralFactory pf;
	GPIOSystem* gpio = (GPIOSystem*) pf->getPeripheral(PERIPHERAL_GPIO);

	//Initialize data
	Display* display = new Display(gpio->getPin(20),
					  gpio->getPin(25),
					  gpio->getPin(8),
					  gpio->getPin(15),
					  gpio->getPin(7),
					  gpio->getPin(18),
					  gpio->getPin(1),
					  gpio->getPin(23),
					  gpio->getPin(12),
					  gpio->getPin(4));
	
	KeyPad* keypad = new KeyPad(
					gpio->getPin(5),
					gpio->getPin(6),
					gpio->getPin(26),
					gpio->getPin(19),
					gpio->getPin(13),
					gpio->getPin(9),
					gpio->getPin(10),
					gpio->getPin(22));
					
	int currentKey = 0;
	
	while((currentKey = keypad->getKey()) < 15)
	{
		if (currentKey > 9)
		{
			currentKey += 7;
		}
		
		display->write((char)(0x30 + currentKey));
	}
	
	return 0;
}
