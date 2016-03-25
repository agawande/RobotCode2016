#ifndef CAGE_HPP
#define CAGE_HPP

#include "conveyor.hpp"
#include "coupling.hpp"

using namespace std;

const unsigned int RED_DEVICE_ID       = 0;
const unsigned int YELLOW_DEVICE_ID    = 1;
const unsigned int GREEN_DEVICE_ID     = 2;
const unsigned int BLUE_DEVICE_ID      = 3;
const unsigned int COUPLING_DEVICE_ID  = 4;

class Cage
{
   public:

      //Constructors
      Cage(const int myAddress) : address(myAddress), deviceId(deviceId),
                                  redConveyor(address, RED_DEVICE_ID),
                                  blueConveyor(address, BLUE_DEVICE_ID), 
                                  greenConveyor(address, GREEN_DEVICE_ID), 
                                  yellowConveyor(address, YELLOW_DEVICE_ID),
                                  coupling(address, COUPLING_DEVICE_ID) 
                                  {}
      
      //Cage Conveyors
      Conveyor redConveyor;
      Conveyor blueConveyor; 
      Conveyor greenConveyor;
      Conveyor yellowConveyor;

      //Cage Coupling
      Coupling coupling;

   private:
      const int address;
      const unsigned int deviceId;
};

#endif
