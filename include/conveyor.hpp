#ifndef CONVEYOR_HPP
#define CONVEYOR_HPP

#include "i2cdispatcher.hpp"
#include "messageformatter.hpp"

//Conveyor states/commands
const unsigned int STOP      = 0;
const unsigned int FORWARD   = 1;
const unsigned int REVERSE   = 2;
const unsigned int SHUFFLE   = 3;
const unsigned int UNDEFINED = -1;

using namespace std; 

//class Conveyor : public I2Cdispatcher {
class Conveyor {
   public: 
      
      //Constructors
      Conveyor(const int myAddress, const unsigned int deviceId) : 
                                                                   address(myAddress),
                                                                   deviceId(deviceId),
                                                                   i2cDispatcher(address) {} 

      //Getters 
      int  getState() { return state; }
      int  getAddress() { return address; }
      unsigned int getDeviceId() { return deviceId; } 

      //Setters
      void setState(unsigned int newState);
      void setDeviceId(unsigned int newId);

      //Public methods
      //There are four separate movement methods in order to avoid if-else statements
      void moveForward();
      void moveReverse();
      void moveShuffle();
      void stop();

   private: 

      //Private variables
      int state; 
      const int address;
      int deviceId;

      //Private methods and objects
      MessageFormatter messageFormatter;
      I2cDispatcher i2cDispatcher;

}; // class Conveyor

#endif
