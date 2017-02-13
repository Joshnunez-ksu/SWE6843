#include "Peripherals.h"
#include <cstring>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/unistd.h>
#include <iostream>


PeripheralFactory::PeripheralFactory()
{
}

PeripheralFactory::~PeripheralFactory()
{
      for (std::map<std::string, Peripheral*>::iterator it=this->peripheralMap.begin();
            it != this->peripheralMap.end();
            ++it)
      {
            delete it->second;
      }
}

Peripheral* PeripheralFactory::getPeripheral(const char* peripheralName)
{
      Peripheral* returnPeripheral = (Peripheral*) 0;
      std::map<std::string, Peripheral*>::iterator it;
      
      if ((it =  this->peripheralMap.find(peripheralName)) == this->peripheralMap.end())
      {
            if (0 == strcmp(peripheralName, PERIPHERAL_GPIO))
            {
                  returnPeripheral = (Peripheral*) new GPIOSystem();
                  this->peripheralMap[PERIPHERAL_GPIO] = returnPeripheral;
            }
            else
            {
                  // no object found for this peripheral name
            }
      }
      else
      {
            returnPeripheral = it->second;
      }
      
      return returnPeripheral;
}

Peripheral::Peripheral(const char* name)
{
      this->name = name;
}

void Peripheral::setRegisterAddress(const char* registerName, volatile unsigned int* address)
{
      // add it to the map
      this->registerMap[registerName] = address;
}

volatile unsigned int* Peripheral::getRegister(const char* registerName)
{
      return this->registerMap[registerName];
}

GPIOSystem::GPIOSystem() : Peripheral(PERIPHERAL_GPIO)
{
      // base on an ARMv7 processor
      //  have to interigate the /proc/cpuinfo if we want to be smart and change the base address base on device we are running on
      unsigned long GPIO_BASE = 0x3F200000;
      
      // initialize the memory
      int memoryHandle;
      if ((this->memoryFD = open("/dev/mem", O_RDWR|O_SYNC)) < 0)
      {
            std::cout << "Failed to get memory handle.\n";
            return;
      }
      
      this->memoryMap = mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, this->memoryFD, GPIO_BASE);
      
      if (this->memoryMap == MAP_FAILED)
      {
            std::cout << "Failed to map memory.\n";
            return;
      }
      
      volatile unsigned int* address = (volatile unsigned int*)this->memoryMap;

      // set all the register addresses
      this->setRegisterAddress(REGISTER_GPIO_GPFSEL, address);
      this->setRegisterAddress(REGISTER_GPIO_GPSET, (address + 7));
      this->setRegisterAddress(REGISTER_GPIO_GPCLR, address + 10);
      this->setRegisterAddress(REGISTER_GPIO_GPLEV, address + 13);
      this->setRegisterAddress(REGISTER_GPIO_GPPUD, address + 37);
      this->setRegisterAddress(REGISTER_GPIO_GPPUDCLK, address + 38);

}

GPIOSystem::~GPIOSystem()
{
      munmap(this->memoryMap, BLOCK_SIZE);
      close(this->memoryFD);
      
      for (std::map<int, GPIOPin*>::iterator it=this->pinMap.begin();
            it != this->pinMap.end();
            ++it)
      {
            delete it->second;
      }
}

GPIOPin* GPIOSystem::getPin(int pinNumber)
{
      GPIOPin* returnPin = (GPIOPin*) 0;
      
      if (28 >= pinNumber && 0 <= pinNumber)
      {
            if (this->pinMap.find(pinNumber) == this->pinMap.end())
            {
                  this->pinMap[pinNumber] = new GPIOPin(this, pinNumber);
            }
            
            returnPin = this->pinMap[pinNumber];
      }
      
      return returnPin;
}

GPIOPin::GPIOPin(GPIOSystem* gpio, int pinNumber)
{
      this->gpio = gpio;
      this->pinNumber = pinNumber;
      
      // get the registers we care about...
      this->GPFSEL = this->gpio->getRegister(REGISTER_GPIO_GPFSEL);
      this->GPSET = this->gpio->getRegister(REGISTER_GPIO_GPSET);
      this->GPCLR = this->gpio->getRegister(REGISTER_GPIO_GPCLR);
      this->GPLEV = this->gpio->getRegister(REGISTER_GPIO_GPLEV);
      
      // default the pin to input...
      this->setDirection(IN);
}

GPIOPin::~GPIOPin()
{
      // clear the pin and set it to input
      this->setValue(LOW);
      this->setDirection(IN);
}

void GPIOPin::setValue(VOLTAGE v)
{
      // use the register and set the voltage
      if (HIGH == v)
      {
            *this->GPSET |= 1 << this->pinNumber;
      }
      else
      {
            *this->GPCLR |= 1 << this->pinNumber;
      }
}

VOLTAGE GPIOPin::getValue()
{
      // use the register and get the voltage
}

void GPIOPin::setDirection(DIRECTION d)
{
      // use the register to set the direction
      if (IN == d)
      {
            *this->GPFSEL &= ~(7 << (this->pinNumber * 3));
      }
      else
      {
            *this->GPFSEL |= 1 << (this->pinNumber * 3);
      }
}
