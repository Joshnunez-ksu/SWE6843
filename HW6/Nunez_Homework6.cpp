/********************************************************************************
Homework 6

********************************************************************************/

#include "../common/Peripherals.h"
#include <iostream>

int main()
{
	PeripheralFactory pf;
	GPIOSystem* gpio = (GPIOSystem*) pf.getPeripheral(PERIPHERAL_GPIO);
	
	GPIOPin* LED1 = gpio->getPin(26);
	GPIOPin* LED2 = gpio->getPin(19);
	GPIOPin* LED3 = gpio->getPin(13);
	GPIOPin* LED4 = gpio->getPin(6);
	
	// inititalize LEDS
	LED1->setDirection(OUT);
	LED2->setDirection(OUT);
	LED3->setDirection(OUT);
	LED4->setDirection(OUT);
	
	LED1->setValue(HIGH);
	LED2->setValue(HIGH);
	LED3->setValue(HIGH);
	LED4->setValue(HIGH);
	
	nanowait(1, 0);
	
	LED1->setValue(LOW);
	LED2->setValue(LOW);
	LED3->setValue(LOW);
	LED4->setValue(LOW);
	// done initializing
	
	// initialize A/D converter
	GPIOPin* csPin		= gpio->getPin(21);
	GPIOPin* dataInPin	= gpio->getPin(20);
	GPIOPin* dataOutPin	= gpio->getPin(16);
	GPIOPin* clockPin	= gpio->getPin(12);

	csPin->setDirection(OUT);
	csPin->setValue(HIGH);

	dataInPin->setDirection(OUT);

	dataOutPin->setDirection(IN);

	clockPin->setDirection(OUT);
	clockPin->setValue(LOW);
	// done initializing
	
	// initialize kill button
	Button killButton(gpio->getPin(25));
	// done initializing

	int clockCount = 0;
	int reading = 0;
	int lastReading = 0;
	
	while (!killButton.pressed())
	{
		lastReading = reading;
		reading = 0;
		
		//start a read from the A/D
		dataInPin->setValue(HIGH);
		csPin->setValue(LOW);
		
		//send configuration bits
		// one null bit and 4 config bits (b1000 means use channel 0)
		for(clockCount = 0; clockCount <= 5; clockCount++)
		{
			if (clockCount > 1)
			{
				dataInPin->setValue(LOW);
			}
			
			clockPin->setValue(HIGH);
			clockPin->setValue(LOW);
			nanowait(0,2000);
		}
		
		//configuration sent now start reading 10 bits of data
		for(clockCount = 10; clockCount >= 0; clockCount--)
		{
			if (clockCount < 10)
			{
				//read the next bit
				if (dataOutPin->getValue() == HIGH)
				{
					reading |= (1 << clockCount);
				}
			}
			
			clockPin->setValue(HIGH);
			clockPin->setValue(LOW);
			nanowait(0,10000);
		}
		
		// done reading
		csPin->setValue(HIGH);
		
		if (lastReading != reading)
		{
			std::cout << reading << "\n";
		}
		
		//0011 1111 1111
		LED1->setValue(HIGH); // LED1 is always on
		
		if (reading > 1024 / 4)
		{
			LED2->setValue(HIGH);
		}
		else
		{
			LED2->setValue(LOW);
		}
		
		if (reading > (1024 / 4) * 2)
		{
			LED3->setValue(HIGH);
		}
		else
		{
			LED3->setValue(LOW);
		}
		
		if (reading > (1024 / 4) * 3)
		{
			LED4->setValue(HIGH);
		}
		else
		{
			LED4->setValue(LOW);
		}
	}
	
	return 0;
}
