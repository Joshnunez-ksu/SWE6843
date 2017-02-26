/********************************************************************************
test the scale
********************************************************************************/

#include <iostream>
#include "../common/Peripherals.h"
#include <time.h>

#define CLOCK_WAIT  2000

long getTick()
{
      struct timespec ts;
      timespec_get(&ts, TIME_UTC);
      
      return (ts.tv_sec * 1000) + (ts.tv_nsec/1000000);
}

class Scale
{
public:
    Scale(GPIOPin* dataPin, GPIOPin* clockPin);
    ~Scale();
    
    long getWeight();
    
private:
    GPIOPin* dataPin;
    GPIOPin* clockPin;
    
    bool isReady();
    void reset();
};

Scale::Scale(GPIOPin* dataPin, GPIOPin* clockPin)
{
    this->dataPin = dataPin;
    this->clockPin = clockPin;
    
    this->dataPin->setDirection(IN);
    
    // reset the scale
    this->clockPin->setDirection(OUT);
    this->reset();
    
    // now waiting for the data pin to go LOW to indicate that the device is ready
    this->clockPin->setValue(LOW);
}

Scale::~Scale()
{
    this->reset();
}

long Scale::getWeight()
{
    long returnWeight = -1;
    
    if (this->isReady())
    {
        
        returnWeight = 0;
        
        for(int i = 23; i >= 0; i--)
        {
            this->clockPin->setValue(HIGH);
            this->clockPin->setValue(LOW);
            nanowait(0, CLOCK_WAIT);
            
            if (this->dataPin->getValue())
            {
                returnWeight |= (1 << i);
            }
        }
        
        // final clock to indicate channel A, gain 128
        this->clockPin->setValue(HIGH);
        this->clockPin->setValue(LOW);
        nanowait(0, CLOCK_WAIT);
        
        if (returnWeight >= 0x7FFFFF && returnWeight <= 0xFFFFFF)
        {
            returnWeight = -1;
        }
    }
    
    return returnWeight;
}

bool Scale::isReady()
{
    bool returnReady = false;
    
    if (!this->dataPin->getValue())
    {
        returnReady = true;
    }
    
    return returnReady;
}

void Scale::reset()
{
    this->clockPin->setValue(LOW);
    nanowait(0, 60000);
    this->clockPin->setValue(HIGH);
    nanowait(0, 100000);
}

int main()
{
    PeripheralFactory pf;
    GPIOSystem* gpio = (GPIOSystem*) pf.getPeripheral(PERIPHERAL_GPIO);

    Scale scale(gpio->getPin(21), gpio->getPin(20));
    
    long startTick = getTick();

    int running = 0;

    long lastValue = 0;
    long currentValue = 0;
    
    int totalReads = 0;
    int goodReads = 0;

    while((getTick() - startTick) < 60000)
    {
        currentValue = scale.getWeight();
        
        if (currentValue != -1)
        {
            if (currentValue > 40000 && currentValue < 50000)
            {
                goodReads++;
            }
            totalReads++;
        }

        if (currentValue != lastValue)
        {
              std::cout << currentValue << "\n";
              std::cout.flush();
        }

        lastValue = currentValue;
    }

    std::cout << "\n" << CLOCK_WAIT << "\n\tTotal Reads: " << totalReads << "\n\t" << "Good Reads: " << goodReads << "\n\t" << "Bad Reads: " << totalReads - goodReads << "\n\t" << "percentage bad: " << (totalReads - goodReads)/totalReads << "\n";

    return 0;
}
