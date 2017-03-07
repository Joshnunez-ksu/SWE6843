#ifndef __PERIPHERALS_H__
#define __PERIPHERALS_H__

#include <map>
#include <string>
#include <bitset>

#include <iostream>
#include <fstream>

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

void nanowait(long seconds, long nanoseconds);

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
      std::string strPortPath;
      
      std::ofstream valueOut;
      std::ifstream valueIn;
      
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
      char buffer;
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

class KeyPad
{
public:
	KeyPad(GPIOPin* pin0, GPIOPin* pin1, GPIOPin* pin2, GPIOPin* pin3, GPIOPin* pin4, GPIOPin* pin5, GPIOPin* pin6, GPIOPin* pin7);
	~KeyPad();

	int getKey();

private:
	GPIOPin*	pins[8];
	
	char		buffer[4];
	int			padGrid[4][4];

};

class Scale
{
public:
    Scale(GPIOPin* dataPin, GPIOPin* clockPin);
    ~Scale();
    
    int     getReading();
    float   getGrams();
    
    void    setOffset(int offset) {this->offset = offset;};
    
private:
    GPIOPin*    dataPin;
    GPIOPin*    clockPin;
    int         offset;
    float       factor;
        
    bool isReady();
    void reset();
};

typedef	std::bitset<2> mode;
typedef	std::bitset<8> data;

class Display 
{
public:
	
	Display(GPIOPin* RegisterSelect, GPIOPin* ReadWrite, GPIOPin* CLK_1, GPIOPin* CLK_2, GPIOPin* DB7, GPIOPin* DB6, GPIOPin* DB5, GPIOPin* DB4, GPIOPin* DB3, GPIOPin* DB2, GPIOPin* DB1, GPIOPin* DB0);
	~Display();

	void		write(const char* Text, int size);
	void		write(std::string Text);
	void		write(std::string Text, long Time);
	void		clear();
	
	
private:
	GPIOPin*	RegisterSelect;
	GPIOPin*	ReadWrite;
	GPIOPin*	CLK_1;
	GPIOPin*	CLK_2;
	GPIOPin*	DataBus[8];
	
	char		characterMap[128];
	/* = {
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',			// 0 - 15
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',			// 16 - 31
		' ', '!', '"', '#', '$', '%', '&',  39, '(', ')', '*', '+', ',', '-', '.', '/',			// 32 - 47
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?',			// 48 - 63
		'@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',			// 64 - 79
		'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', 165, ']', '^', 95,			// 80 - 05
		 96, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',			// 96 - 111
		'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', ' ', ' '			// 112 - 127
	};
	*/
	
	enum 		STATE {
		TOP = 1, BOTTOM = 2
	};
	
	// Variables
	STATE		ACTIVE_CLOCK; // = TOP;
	mode		CURRENT_MODE;
	data		DDRAM_ADDRESS; // = 0x00;
	
	// Internal Operations
	void		operate(STATE Enabler, mode Mode, data Instruction, long Duration);
	void		setMode(mode);
	mode		getMode();
	void		checkBusy(STATE Enabler);
	void		writeToDDRAM(STATE Enabler, data Data);
	void		switchActiveClock();
};

#endif
