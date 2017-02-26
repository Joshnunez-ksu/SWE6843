#include "Peripherals.h"
#include <cstring>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/unistd.h>
#include <iostream>
#include <fstream>

using namespace std;

void nanowait(long seconds, long nanoseconds)
{
      
      timespec ts;
      ts.tv_sec = seconds;
      ts.tv_nsec = nanoseconds;
      
      nanosleep(&ts, &ts);
}

PeripheralFactory::PeripheralFactory()
{
}

PeripheralFactory::~PeripheralFactory()
{
      for (std::map<std::string, Peripheral*>::iterator it=this->peripheralMap.begin();
            it != this->peripheralMap.end();
            ++it)
      {
            if (it->second->getName() == PERIPHERAL_GPIO)
            {
                  delete ((GPIOSystem*) it->second);
            }
            else if (it->second->getName() == PERIPHERAL_PWM)
            {
                  delete ((PWMSystem*) it->second);
            }
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
      for (std::map<int, GPIOPin*>::iterator it=this->pinMap.begin();
            it != this->pinMap.end();
            ++it)
      {
            delete (GPIOPin*) it->second;
      }
      
      munmap(this->memoryMap, BLOCK_SIZE);
      close(this->memoryFD);
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

/*
//mem map version
GPIOPin::GPIOPin(GPIOSystem* gpio, int pinNumber)
{
      this->gpio = gpio;
      this->pinNumber = pinNumber;
      
      // get the registers we care about...
      this->GPFSEL = this->gpio->getRegister(REGISTER_GPIO_GPFSEL);
      this->GPSET = this->gpio->getRegister(REGISTER_GPIO_GPSET);
      this->GPCLR = this->gpio->getRegister(REGISTER_GPIO_GPCLR);
      this->GPLEV = this->gpio->getRegister(REGISTER_GPIO_GPLEV);
      this->GPPUD = this->gpio->getRegister(REGISTER_GPIO_GPPUD);
      this->GPPUDCLK = this->gpio->getRegister(REGISTER_GPIO_GPPUDCLK);
      
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
            *this->GPSET |= (1 << this->pinNumber);
      }
      else
      {
            *this->GPCLR |= (1 << this->pinNumber);
      }
}

VOLTAGE GPIOPin::getValue()
{
      // use the register and get the voltage
      return (VOLTAGE) ((*this->GPLEV & (1 << this->pinNumber)) >> this->pinNumber);
}

void GPIOPin::setDirection(DIRECTION d)
{
      // use the register to set the direction
      if (IN == d)
      {
            *(this->GPFSEL + (this->pinNumber/10)) &= ~(7 << ((this->pinNumber%10) * 3));
      }
      else
      {
            *(this->GPFSEL + (this->pinNumber/10)) |= 1 << ((this->pinNumber%10) * 3);
      }
}

void GPIOPin::setPull(PULL p)
{
      *this->GPPUD = 0;
      *this->GPPUDCLK = 0;
      
      *this->GPPUD = 0x01;
      
      if (UP)
      {
            *this->GPPUD = 0x02;
      }
      
      nanowait(255000000);
      *this->GPPUDCLK |= 1 << this->pinNumber;
      nanowait(255000000);
      
      *this->GPPUD = 0;
      *this->GPPUDCLK = 0;
}
*/

const char* GPIO_FILE_PATH = "/sys/class/gpio";

// sysfs version of GPIO
GPIOPin::GPIOPin(GPIOSystem* s, int pinNumber)
{
      this->strBasePath = GPIO_FILE_PATH;
      this->pinNumber = pinNumber;
      sprintf(&this->strPinNumber[0], "%d", this->pinNumber);
      this->strPortPath = this->strBasePath + "/gpio" + this->strPinNumber;
      
	// export the GPIO pins
	ofstream exportFile(this->getGPIOPath("/export").c_str());
	exportFile << this->strPinNumber;;
	exportFile.close();
	
	// set direction
	/*
	ofstream dirFile(this->getPortPath("/direction").c_str());
	dirFile << direction;
	dirFile.flush();
	dirFile.close();
	*/
}

GPIOPin::~GPIOPin()
{
      // set the port to input to guard against shorts
      ofstream pin((this->strPortPath + "/direction").c_str());
      pin << "in";
      pin.close();
      
      // unexport the port
      ofstream unexport(this->getGPIOPath("/unexport").c_str());
      unexport << this->strPinNumber;
      unexport.close();
}

VOLTAGE GPIOPin::getValue()
{
      ifstream pin(this->getPortPath("/value").c_str());
      
      char v;
      pin >> v;
      
      pin.close();
      
      return (VOLTAGE) (v & ~0x30);
}

void GPIOPin::setValue(VOLTAGE value)
{
      ofstream pin(this->getPortPath("/value").c_str());
      pin << (char) ((int)value | 0x30);
      pin.flush();
      
      pin.close();
}

void GPIOPin::setDirection(DIRECTION d)
{
	ofstream dirFile(this->getPortPath("/direction").c_str());
	dirFile << (d == IN?"in":"out");
	dirFile.flush();
	dirFile.close();
}

void GPIOPin::setPull(PULL p)
{
}

string GPIOPin::getGPIOPath(const char* path)
{
      return this->strBasePath + path;
}

string GPIOPin::getPortPath(const char* path)
{
      string retVal = this->strBasePath + "/gpio" + this->strPinNumber + path;
      return retVal;
}

PWMSystem::PWMSystem() : Peripheral(PERIPHERAL_PWM)
{
/*
      // base on an ARMv7 processor
      //  have to interigate the /proc/cpuinfo if we want to be smart and change the base address base on device we are running on
      unsigned long CLOCK_BASE = 0x3F20C000;
      unsigned long PWM_BASE = 0x3F101000;
      
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
*/
}

PWMSystem::~PWMSystem()
{
}
      
PWMPin* PWMSystem::getPin(int pinNumber)
{
      PWMPin* returnPin = (PWMPin*)0;
      
      return returnPin;
}

PWMPin::PWMPin(PWMSystem* pwm, int pinNumber)
{
}

PWMPin::~PWMPin()
{
}
      
void PWMPin::setM(int m)
{
}

void PWMPin::setN(int n)
{
}

Button::Button(GPIOPin* pin)
{
      this->pin = pin;
      this->pin->setDirection(IN);
      this->buffer = 0xe0;
}

bool Button::pressed()
{
      this->buffer = (this->buffer << 1) | (char)this->pin->getValue() | 0xe0;
      if (this->buffer == 0xf0)
      {
            this->buffer = 0xe0;
            return true;
      }
      else
      {
            nanowait(0, 1000000);
      }
      
      return false;
}

SingleDigitDisplay::SingleDigitDisplay(GPIOPin* pin0, GPIOPin* pin1, GPIOPin* pin2, GPIOPin* pin3, GPIOPin* dp)
{
      this->pin0 = pin0;
      this->pin1 = pin1;
      this->pin2 = pin2;
      this->pin3 = pin3;
      this->dp = dp;
      
      this->pin0->setDirection(OUT);
      this->pin1->setDirection(OUT);
      this->pin2->setDirection(OUT);
      this->pin3->setDirection(OUT);
      this->dp->setDirection(OUT);
      
      this->pin0->setValue(LOW);
      this->pin1->setValue(LOW);
      this->pin2->setValue(LOW);
      this->pin3->setValue(LOW);
      this->dp->setValue(LOW);
}

void SingleDigitDisplay::setDisplay(int displayValue)
{
      //std::cout << this->pin0->getValue() << this->pin1->getValue() << this->pin2->getValue() << this->pin3->getValue() << "\n";
      
      if (9 >= displayValue && displayValue >= 0)
      {
/*            
            if (this->pin0->getValue() == LOW)
            {
                  this->pin0->setValue(HIGH);
            }
            else
            {
                  this->pin0->setValue(LOW);
            }
*/

            pin0->setValue((displayValue & 0x01)?HIGH:LOW);
            pin1->setValue((displayValue & 0x02)?HIGH:LOW);
            pin2->setValue((displayValue & 0x04)?HIGH:LOW);
            pin3->setValue((displayValue & 0x08)?HIGH:LOW);

      }
}

void SingleDigitDisplay::setDecimal(VOLTAGE v)
{
      this->dp->setValue(v);
}

Buzzer::Buzzer(PWMPin* pin)
{
}

// period = 1 / frequency
SoftBuzzer::SoftBuzzer(GPIOPin* gpio)
{
      this->pin = gpio;
      this->pin->setDirection(OUT);
      
      this->mFrequency = 0;
}

void SoftBuzzer::setFrequency(int f)
{
      this->mFrequency = f;
}

void SoftBuzzer::enable(long duration)
{
      int seconds = duration / 1000;
      int milliseconds = duration - (seconds * 1000);
      
      this->pin->setValue(HIGH);
      nanowait(seconds, milliseconds * 1000000);
      this->pin->setValue(LOW);
      
      /*
      bool on = true;

      this->pin->setValue(HIGH);
      long timesPerSec = 1000000/this->mFrequency;
      
      int loopCount = ((duration / 1000) * this->mFrequency) * 2;
      
      timespec ts;
      ts.tv_sec = 0;
      ts.tv_nsec = timesPerSec / 2;
            
      while (loopCount-- > 0)
      {
            on = !on;
            nanosleep(&ts, &ts);
            this->pin->setValue((VOLTAGE) on );
      }
      
      this->pin->setValue(LOW);
      */
}

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
