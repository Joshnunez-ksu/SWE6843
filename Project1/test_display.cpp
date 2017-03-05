#include <iostream>
#include "/home/pi/GroupWork/common/Peripherals.cpp"

using namespace std;

int main(int argc, char **argv)
{
	PeripheralFactory pf;
	GPIOSystem* gpio = (GPIOSystem*) pf.getPeripheral(PERIPHERAL_GPIO);

	Display display(gpio->getPin(20), gpio->getPin(25), gpio->getPin(16), gpio->getPin(21), gpio->getPin(8), gpio->getPin(15), gpio->getPin(7), gpio->getPin(18), gpio->getPin(1), gpio->getPin(23), gpio->getPin(12), gpio->getPin(24));
	
	string text = "Write whatever you like.";
	
	display.write(text);
	
	while(true)
	{
		getline(cin, text);
		display.write(text);
	}
}
