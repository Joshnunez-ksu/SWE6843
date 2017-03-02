/********************************************************************************
test the scale
********************************************************************************/

#include <iostream>
#include "../common/Peripherals.h"
#include <time.h>
#include <cstdlib>

#define CLOCK_WAIT  2000

long getTick()
{
      struct timespec ts;
      timespec_get(&ts, TIME_UTC);
      
      return (ts.tv_sec * 1000) + (ts.tv_nsec/1000000);
}

struct AverageBufferElement
{
    int data;
    AverageBufferElement* next;
    AverageBufferElement* prev;
};

class AverageBuffer
{
public:
    AverageBuffer(int size);
    ~AverageBuffer();
    
    int getAverage();
    void addValue(int v);
    
private:
    AverageBufferElement* head;
    AverageBufferElement* last;
    int maxSize;
    int size;
};

AverageBuffer::AverageBuffer(int size)
{
    this->maxSize = size;
    this->head = (AverageBufferElement*)0;
    this->last = (AverageBufferElement*)0;
}

AverageBuffer::~AverageBuffer()
{
    AverageBufferElement* current = this->head;
    AverageBufferElement* temp;
    
    while(current)
    {
        temp = current->next;
        delete current;
        
        current = temp;
    }
}

void AverageBuffer::addValue(int v)
{
    AverageBufferElement* newElement = new AverageBufferElement;
    newElement->data = v;
    newElement->prev = (AverageBufferElement*) 0;
    newElement->next = (AverageBufferElement*) 0;
    
    // empty buffer
    if (!this->head)
    {
        this->head = newElement;
        this->last = this->head;
        
        this->size++;
    }
    else
    {
        this->last->next = newElement;
        newElement->prev = this->last;        
        this->last = newElement;
        
        // see if we are at the max size
        if (this->maxSize <= this->size)
        {
            AverageBufferElement* old = this->head;
            this->head = this->head->next;
            this->head->prev = (AverageBufferElement*)0;
            
            delete old;
        }
        else
        {
            this->size++;
        }
    }
}

int AverageBuffer::getAverage()
{
    long sum = 0;
    
    AverageBufferElement* current = this->head;
    
    while (current)
    {
        //std::cout << "[" << current->data << "]";
        sum += current->data;
        current = current->next;
    }
    
    //std::cout << "\n";
    
    if (sum == 0 || this->size == 0)
    {
        return 0;
    }
    
    return sum / this->size;
}

class Scale
{
public:
    Scale(GPIOPin* dataPin, GPIOPin* clockPin);
    ~Scale();
    
    int getWeight();
    int getAverageWeight();
    
private:
    GPIOPin* dataPin;
    GPIOPin* clockPin;
    AverageBuffer* buffer;
    
    bool isReady();
    void reset();
};

Scale::Scale(GPIOPin* dataPin, GPIOPin* clockPin)
{
    this->buffer = new AverageBuffer(5);
    
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

int Scale::getWeight()
{
    int returnWeight = -1;
    
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

int Scale::getAverageWeight()
{
    int currentValue = this->getWeight();
    int currentAverage = this->buffer->getAverage();
    
    if ( currentValue != -1 &&
        (currentAverage == 0 || abs(currentValue - currentAverage) < (currentAverage*2)) )
    {
        this->buffer->addValue(currentValue);
    }
    
    return currentAverage;
}

/*
int Scale::getAverageWeight()
{
    // smooth out the readings
    // Looks for three values that are reasonable close to each other
    
    int refWeight = -1;
    long weightSum = 0;
    
    int weights[4];
    
    weights[0] = this->getWeight();
    weights[1] = this->getWeight();
    weights[2] = this->getWeight();
    weights[3] = this->getWeight();
    
    for (int i = 0; i < 4; i++)
    {
        if (refWeight == -1)
        {
            if (weights[i] != -1)
            {
                refWeight = weights[i];
                weightSum = (refWeight) + (refWeight * i);
            }
        }
        else
        {
            if (abs(refWeight - weights[i]) > (refWeight / 4))
            {
                weightSum += refWeight;
            }
            else
            {
                weightSum += weights[i];
            }
        }
    }
    
    if (refWeight == -1) return refWeight;
    
    return weightSum / 4;
}
*/

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

    int lastValue = 0;
    int currentValue = 0;
    
    int totalReads = 0;
    int goodReads = 0;

    while((getTick() - startTick) < 60000)
    {
        //currentValue = scale.getWeight();
        currentValue = scale.getAverageWeight();

        if (currentValue != -1)
        {
            if (currentValue > 40000 && currentValue < 50000)
            {
                goodReads++;
            }
            totalReads++;
            
//            if (currentValue != lastValue)
//            {
                  //std::cout << (currentValue & 0xFFFFFC00) << "\n";
                  //std::cout << currentValue << "\t" << getTick() - startTick << "\n";
                  std::cout << currentValue << "\r";
                  std::cout.flush();
//            }

            lastValue = currentValue;
        }
        
        //nanowait(0,250000000);

    }

    std::cout << "\n" << CLOCK_WAIT << "\n\tTotal Reads: " << totalReads << "\n\t" << "Good Reads: " << goodReads << "\n\t" << "Bad Reads: " << totalReads - goodReads << "\n\t" << "percentage bad: " << (totalReads - goodReads)/totalReads << "\n";

    return 0;
}
