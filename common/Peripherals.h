#ifndef __PERIPHERALS_H__
#define __PERIPHERALS_H__

class Peripheral
{
public:
      Peripheral();
      
      virtual char* getName() = 0;
};

class PeripheralFactory
{
public:
      PeripheralFactory();
      Peripheral* getPeripheral(const char* peripheralName);
      
private:
      
};

#endif
