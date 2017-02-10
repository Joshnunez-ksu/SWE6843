#include "GPIO-work.h"
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <time.h>

const char* GPIO_FILE_PATH = "/sys/class/gpio";

GPIO::GPIO(const char* portNumber, const char* direction)
{
      this->strBasePath = GPIO_FILE_PATH;
      this->strPortNumber = portNumber;
      
	// export the GPIO pins
	ofstream exportFile(this->getGPIOPath("/export").c_str());
	exportFile << this->strPortNumber.c_str();;
	exportFile.close();
	
	// set direction
	ofstream dirFile(this->getPortPath("/direction").c_str());
	dirFile << direction;
	dirFile.flush();
	dirFile.close();
	
}

GPIO::~GPIO()
{
      // set the port to input to guard against shorts
      ofstream pin(this->getPortPath("/direction").c_str());
      pin << "in";
      pin.close();
      
      // unexport the port
      ofstream unexport(this->getGPIOPath("/unexport").c_str());
      unexport << this->strPortNumber.c_str();
      unexport.close();
}

int GPIO::getValue()
{
      ifstream pin(this->getPortPath("/value").c_str());
      
      char v;
      pin >> v;
      
      pin.close();
      
      return v & ~0x30;
}

void GPIO::setValue(int value)
{
      ofstream pin(this->getPortPath("/value").c_str());
      pin << (char) (value | 0x30);
      pin.flush();
      
      pin.close();
}

string GPIO::getGPIOPath(const char* path)
{
      return this->strBasePath + path;
}

string GPIO::getPortPath(const char* path)
{
      string retVal = this->strBasePath + "/gpio" + this->strPortNumber + path;
      return retVal;
}

long getTick()
{
      struct timespec ts;
      timespec_get(&ts, TIME_UTC);
            
      return (ts.tv_sec * 1000) + (ts.tv_nsec/1000000);
}

// period = 1 / frequency
SoftPWM::SoftPWM(GPIO* gpio)
{
      this->mGPIO = gpio;
      
      this->mDuty = 0;
      this->mPeriod = 0;
      this->mStartTick = 0;
}

void SoftPWM::setDuty(long duty)
{
      this->mDuty = duty;
}

void SoftPWM::setPeriod(long period)
{
      this->mPeriod = period;
}

void SoftPWM::enable(long duration)
{
      bool on = true;
            
      this->mGPIO->setValue((int) on);
      
      timespec ts;
      ts.tv_sec = 0;
      ts.tv_nsec = this->mPeriod;
      
      long elapsedStartTime = getTick();
            
      while ((getTick() - elapsedStartTime) < duration)
      {
            on = !on;
            nanosleep(&ts, &ts);
            this->mGPIO->setValue((int) on );
      }
      
      this->mGPIO->setValue(0);
}

long SoftPWM::getElapsedTime()
{
      return getTick() - this->mStartTick;
}

Button::Button(GPIO* gpio)
{
      this->mGPIO = gpio;
}

int Button::getState()
{
      timespec ts;
      ts.tv_sec = 0;
      ts.tv_nsec = 500;
 
      //delay to account for any bounce
      nanosleep(&ts, &ts);
      
      return this->mGPIO->getValue();
}
