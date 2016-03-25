#include "clamper.hpp"

void Clamper::open()
{
  i2cDispatcher.sendData(messageFormatter.buildMessage(deviceId, OPEN));
  //WAIT FOR COMPLETION HERE??
  //i2cDispatcher.checkForCompletion();
  state = OPEN;
}  //end of open

void Clamper::close()
{
  i2cDispatcher.sendData(messageFormatter.buildMessage(deviceId, CLOSE));
  //???i2cDispatcher.checkForCompletion();
  state = CLOSE;
} //end of close
