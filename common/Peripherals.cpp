#include "Peripherals.h"

Peripheral::Peripheral()
{
}

PeripheralFactory::PeripheralFactory()
{
}

Peripheral* PeripheralFactory::getPeripheral(const char* peripheralName)
{
      return (Peripheral*) 0;
}
