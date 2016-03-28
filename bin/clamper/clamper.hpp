#ifndef CLAMPER_HPP
#define CLAMPER_HPP

#include "i2cdispatcher.hpp"
#include "messageformatter.hpp"

//state/commands
const int CLAMPER_OPEN  = 0;
const int CLAMPER_CLOSE = 1;

class Clamper {
  public:

     //Constructors
     Clamper(const int myAddress, const unsigned int deviceId):
             address(myAddress), deviceId(deviceId), i2cDispatcher(address) {}

     //Getters
     int getState() const { return state; }

     //Public functions
     void open();
     void close();

  private:

      //Private variables
      const int address;
      int state;
      int deviceId;

      //Private methods and objects
      MessageFormatter messageFormatter;
      I2cDispatcher i2cDispatcher;
};

#endif
