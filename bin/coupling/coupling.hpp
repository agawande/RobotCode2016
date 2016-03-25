#ifndef COUPLING_HPP
#define COUPLING_HPP

#include "messageformatter.hpp"
#include "i2cdispatcher.hpp"

const unsigned int COUPLE    = 0;
const unsigned int DECOUPLED = 1;

class Coupling {

   public: //Constructors
      Coupling(const int myAddress, const unsigned int deviceId) : address(myAddress), deviceId(deviceId),
                                                                   i2cDispatcher(address) {}

      //Getters & Setters
      int getState() { return state; }

      //Methods
      void couple();
      void decouple();

   private: 
      
      //Private Data
      int address;
      unsigned int state;
      unsigned int deviceId;

      //Private objects
      MessageFormatter messageFormatter;
      I2cDispatcher i2cDispatcher; 

}; //class Conveyor

#endif
