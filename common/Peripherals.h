#ifndef __PERIPHERALS_H__
#define __PERIPHERALS_H__

#include <map>
#include <string>

#define PERIPHERAL_GPIO  "GPIO"
#define PERIPHERAL_PWM "PWM"

// GPIO System Register Names
#define REGISTER_GPIO_GPFSEL  "GPFSEL"
#define REGISTER_GPIO_GPSET   "GPSET"
#define REGISTER_GPIO_GPCLR   "GPCLR"
#define REGISTER_GPIO_GPLEV   "GPLEV"
#define REGISTER_GPIO_GPPUD   "GPPUD"
#define REGISTER_GPIO_GPPUDCLK      "GPPUDCLK"

#define BLOCK_SIZE      (4*1024)

void nanowait(long nanoseconds);

class Peripheral
{
public:
      Peripheral(const char* name);
      
      std::string getName() {return this->name;};
      volatile unsigned int* getRegister(const char* registerName);
      
protected:
      void setRegisterAddress(const char* registerName, volatile unsigned int* address);
      int memoryFD;
      
private:
      std::string name;
      std::map<std::string, volatile unsigned int*> registerMap;
};

class PeripheralFactory
{
public:
      PeripheralFactory();
      ~PeripheralFactory();
      Peripheral* getPeripheral(const char* peripheralName);
      
protected:
      
private:
      std::map<std::string, Peripheral*> peripheralMap;
};

class GPIOPin;

class GPIOSystem : public Peripheral
{
public:
      GPIOSystem();
      ~GPIOSystem();
      
      GPIOPin* getPin(int pinNumber);
      
 private:
      void* memoryMap;
      int memoryFD;
      std::map<int, GPIOPin*> pinMap;
};

class PWMPin;

class PWMSystem: public Peripheral
{
public:
      PWMSystem();
      ~PWMSystem();
      
      PWMPin* getPin(int pinNumer);
      
private:
      std::map<int, PWMPin*> pinMap;
      
};

enum VOLTAGE
{
      LOW = 0,
      HIGH = 1
};

enum DIRECTION
{
      IN = 0,
      OUT = 1
};

enum PULL
{
      DOWN = 0,
      UP = 0
};

class GPIOPin
{
public:
      GPIOPin(GPIOSystem* gpio, int pinNumber);
      ~GPIOPin();
      
      void setValue(VOLTAGE v);
      VOLTAGE getValue();
      
      void setDirection(DIRECTION d);
      void setPull(PULL p);
      
private:
      std::string getGPIOPath(const char* path);
      std::string getPortPath(const char* path);
      
      GPIOSystem* gpio;
      int pinNumber;
      char strPinNumber[3];
      
      volatile unsigned int* GPFSEL;
      volatile unsigned int* GPSET;
      volatile unsigned int* GPCLR;
      volatile unsigned int* GPLEV;
      volatile unsigned int* GPPUD;
      volatile unsigned int* GPPUDCLK;
      
      std::string strBasePath;
};

class PWMPin
{
public:
      PWMPin(PWMSystem* pwm, int pinNumber);
      ~PWMPin();
      
      void setM(int m);
      void setN(int n);

private:
      int pinNumber;
      PWMSystem* pwm;
      
};

class Button
{
public:
      Button(GPIOPin* pin);
      bool pressed();
      
private:
      GPIOPin* pin;
};

class SingleDigitDisplay
{
public:
      SingleDigitDisplay(GPIOPin* pin0, GPIOPin* pin1, GPIOPin* pin2, GPIOPin* pin3, GPIOPin* dp);
      void setDisplay(int displayValue);
      void setDecimal(VOLTAGE v);
      
private:
      GPIOPin* pin0;
      GPIOPin* pin1;
      GPIOPin* pin2;
      GPIOPin* pin3;
      GPIOPin* dp;
      
};

class Buzzer
{
public:
      Buzzer(PWMPin* pin);

private:
      PWMPin* pin;
};

class SoftBuzzer
{
public:
      SoftBuzzer(GPIOPin* pin);
      
      void setFrequency(int f);
      void enable(long d);
      
private:
      GPIOPin* pin;
      int mFrequency;
      
};

#endif
