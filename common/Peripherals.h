#ifndef __PERIPHERALS_H__
#define __PERIPHERALS_H__

#include <map>
#include <string>

#define PERIPHERAL_GPIO  "GPIO"

// GPIO System Register Names
#define REGISTER_GPIO_GPFSEL  "GPFSEL"
#define REGISTER_GPIO_GPSET   "GPSET"
#define REGISTER_GPIO_GPCLR   "GPCLR"
#define REGISTER_GPIO_GPLEV   "GPLEV"
#define REGISTER_GPIO_GPPUD   "GPPUD"
#define REGISTER_GPIO_GPPUDCLK      "GPPUDCLK"

#define BLOCK_SIZE      (4*1024)

class Peripheral
{
public:
      Peripheral(const char* name);
      
      std::string getName() {return this->name;};
      volatile unsigned int* getRegister(const char* registerName);
      
protected:
      void setRegisterAddress(const char* registerName, volatile unsigned int* address);
      
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

typedef enum
{
      LOW = 0,
      HIGH = 1
} VOLTAGE;

typedef enum
{
      IN = 0,
      OUT = 1
} DIRECTION;

class GPIOPin
{
public:
      GPIOPin(GPIOSystem* gpio, int pinNumber);
      ~GPIOPin();
      
      void setValue(VOLTAGE v);
      VOLTAGE getValue();
      
      void setDirection(DIRECTION d);
      
private:
      GPIOSystem* gpio;
      int pinNumber;
      
      volatile unsigned int* GPFSEL;
      volatile unsigned int* GPSET;
      volatile unsigned int* GPCLR;
      volatile unsigned int* GPLEV;
};

#endif
