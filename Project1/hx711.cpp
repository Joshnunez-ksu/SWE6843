#include <iostream>
#include "hx711.h"
#include "common/Peripherals.cpp"

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

PeripheralFactory pf;
GPIOSystem* gpio = (GPIOSystem*) pf.getPeripheral(PERIPHERAL_GPIO);

GPIOPin* slkPin;
GPIOPin* doutPin;

Hx711::Hx711(uint8_t _pin_dout, uint8_t _pin_slk) : pin_dout(_pin_dout), pin_slk(_pin_slk)
{
	slkPin = gpio->getPin(pin_slk);
      	slkPin->setDirection(OUT);

	doutPin = gpio->getPin(pin_dout);
	doutPin->setDirection(IN);

	slkPin->setValue(HIGH);
	nanowait(0, 100000);
	slkPin->setValue(LOW);

	averageValue();
	this->setOffset(averageValue());
	this->setScale();
	
}

/*Hx711::~Hx711()
{

}*/

long Hx711::averageValue(uint8_t times)
{
	long sum = 0;
	for(uint8_t i = 0; i < times; i++)
	{
		sum += getValue();
	}

	return sum/times;
}

long Hx711::getValue()
{
	uint8_t data[3];

	while(doutPin->getValue());

	for(uint8_t j = 3; j--;)
	{
		for(char i = 8; i--;)
		{
			slkPin->setValue(HIGH);
			bitWrite(data[j], i, doutPin->getValue());
			slkPin->setValue(LOW);
		}
	}

	slkPin->setValue(HIGH);
	slkPin->setValue(LOW);

	data[2] ^= 0x80;

	return ((uint32_t) data[2] << 16) | ((uint32_t) data[1] << 8) | (uint32_t) data[0];
}

void Hx711::setOffset(long _offset)
{
	offset = _offset;
}

void Hx711::setScale(float _scale)
{
	scale = _scale;
}

float Hx711::getGram()
{
	long val = (averageValue() - offset);
	return (float) val / scale;
}

int main()
{
	cout << "Program running!" << endl;
	Hx711 scale(21, 20);
	while(true)
	{
		cout << scale.getGram() << 'g' <<endl;
		nanowait(0, 500000000);
	}
	return 0;
}