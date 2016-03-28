#include "clamper.hpp"

void Clamper::open()
{
  i2cDispatcher.sendData(messageFormatter.buildMessage(deviceId, CLAMPER_OPEN));
  //WAIT FOR COMPLETION HERE??
  //i2cDispatcher.checkForCompletion();
  state = CLAMPER_OPEN;
}  //end of open

void Clamper::close()
{
  i2cDispatcher.sendData(messageFormatter.buildMessage(deviceId, CLAMPER_CLOSE));
  //???i2cDispatcher.checkForCompletion();
  state = CLAMPER_CLOSE;
} //end of close
