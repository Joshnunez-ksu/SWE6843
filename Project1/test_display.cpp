#include <iostream>
#include "../common/Peripherals.h"
//~ #include <
//~ #include "Display.cpp"

using namespace std;

int main(int argc, char **argv)
{
	PeripheralFactory pf;
	GPIOSystem* gpio = (GPIOSystem*) pf.getPeripheral(PERIPHERAL_GPIO);

	//~ Display display(	gpio->getPin(20),
						//~ gpio->getPin(25), 
						//~ gpio->getPin(16), 
						//~ gpio->getPin(21), 
						//~ gpio->getPin(8), 
						//~ gpio->getPin(15), 
						//~ gpio->getPin(7), 
						//~ gpio->getPin(18), 
						//~ gpio->getPin(1), 
						//~ gpio->getPin(23), 
						//~ gpio->getPin(12), 
						//~ gpio->getPin(24));
	
	Display* display = new Display(	gpio->getPin(20),
									gpio->getPin(25), 
									gpio->getPin(16), 
									gpio->getPin(21), 
									gpio->getPin(8), 
									gpio->getPin(15), 
									gpio->getPin(7), 
									gpio->getPin(18), 
									gpio->getPin(1), 
									gpio->getPin(23), 
									gpio->getPin(12), 
									gpio->getPin(24));
	
	
	string text = "Write whatever you like.";
	
	while(text != "exit")
	{
		cout << text << endl;
		display->write(text);
		getline(cin, text);
		//~ cout << text << endl;
	}
	
	display->write("Goodbye!");
	//~ sleep(3);
	display->clear();
	
	return 0;
}
