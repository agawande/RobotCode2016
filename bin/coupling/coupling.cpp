#include "coupling.hpp"

void Coupling::couple()
{
   i2cDispatcher.sendData(messageFormatter.buildMessage(deviceId, COUPLE));

   state = COUPLE;
} //couple


void Coupling::decouple()
{
   i2cDispatcher.sendData(messageFormatter.buildMessage(deviceId, COUPLE));

   state = DECOUPLED;
} //decouple
