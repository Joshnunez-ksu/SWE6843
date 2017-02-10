#ifndef GPIO__H__
#define GPIO__H__

#include <string>

using namespace std;

class GPIO
{
public:
	GPIO(const char* portNumber, const char* direction);
	~GPIO();
	
	int getValue();
	void setValue(int value);

private:
      string strPortNumber;
      string strBasePath;
      
      string getGPIOPath(const char* path);
      string getPortPath(const char* path);
};

class SoftPWM
{
public:
      SoftPWM(GPIO* gpio);
      
      void setDuty(long duty);
      void setPeriod(long period);
      void enable(long duration);
      
private:
      GPIO* mGPIO;
      long  mDuty;
      long  mPeriod;
      long mStartTick;
      
      long getElapsedTime();
      
};

class Button
{
public:
      Button(GPIO* gpio);
      
      int getState();
private:
      GPIO* mGPIO;
      
};
#endif
