/********************************************************************************
Project One
********************************************************************************/

#include <iostream>
#include "../common/Peripherals.h"
#include <time.h>

long getTick()
{
      struct timespec ts;
      timespec_get(&ts, TIME_UTC);
      
      return (ts.tv_sec * 1000) + (ts.tv_nsec/1000000);
}

int main()
{
      PeripheralFactory pf;
      GPIOSystem* gpio = (GPIOSystem*) pf.getPeripheral(PERIPHERAL_GPIO);
      
      GPIOPin* scalePin = gpio->getPin(21);
      scalePin->setDirection(IN);
      
      GPIOPin* clockPin = gpio->getPin(20);
      clockPin->setDirection(OUT);
      clockPin->setValue(HIGH);
      nanowait(0,100000);
      
      std::cout << "Start reading from pin 21\n";
      
      long startTick = getTick();

      int running = 0;
      
      int lastValue = 0;
      int currentValue = 0;
      
      while((getTick() - startTick) < 60000)
      {
            clockPin->setValue(LOW);
            nanowait(0, 2000);
            currentValue = scalePin->getValue();
            
            if (currentValue != lastValue)
            {
                  std::cout << currentValue;
                  std::cout.flush();
            }
            
            lastValue = currentValue;
                   
            clockPin->setValue(HIGH);
      }
      
      std::cout << "\n";
      
      return 0;
}
