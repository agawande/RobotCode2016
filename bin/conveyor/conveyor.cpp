#include "conveyor.hpp"
#include <iostream>

using namespace std; 

/**********************************************************************
 ******************** Public Function Definitions ********************
 **********************************************************************/

void Conveyor::stop()
{
   i2cDispatcher.sendData(messageFormatter.buildMessage(deviceId, STOP));

   state = STOP;
   
} //stop


void Conveyor::moveForward()
{
   i2cDispatcher.sendData(messageFormatter.buildMessage(deviceId, FORWARD));

   state = FORWARD;

} //moveForward


void Conveyor::moveReverse()
{
   i2cDispatcher.sendData(messageFormatter.buildMessage(deviceId, REVERSE));

   state = REVERSE;

} //moveReverse


void Conveyor::moveShuffle()
{
   i2cDispatcher.sendData(messageFormatter.buildMessage(deviceId, SHUFFLE));

   state = SHUFFLE;

} //moveShuffle
