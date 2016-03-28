#ifndef CAGE_HPP
#define CAGE_HPP

#include "conveyor.hpp"
#include "coupling.hpp"

using namespace std;

const unsigned int RED_DEVICE_ID       = 1; // DO NOT CHANGE!!!!
const unsigned int BLUE_DEVICE_ID      = 4; // DO NOT CHANGE!!!!
const unsigned int GREEN_DEVICE_ID     = 3; // DO NOT CHANGE!!!!
const unsigned int YELLOW_DEVICE_ID    = 2; // DO NOT CHANGE!!!!
const unsigned int COUPLING_DEVICE_ID  = 6; // DO NOT CHANGE!!!!

class Cage
{
   public:

      //Constructors
      Cage(const int myAddress) : address(myAddress),
                                  redConveyor(myAddress, RED_DEVICE_ID),
                                  blueConveyor(myAddress, BLUE_DEVICE_ID),
                                  greenConveyor(myAddress, GREEN_DEVICE_ID),
                                  yellowConveyor(myAddress, YELLOW_DEVICE_ID),
                                  coupling(myAddress, COUPLING_DEVICE_ID)
                                  {}

      //Cage Conveyors
      Conveyor redConveyor;
      Conveyor blueConveyor;
      Conveyor greenConveyor;
      Conveyor yellowConveyor;

      //Cage Coupling
      Coupling coupling;

      int getAddress() const {return address;}

   private:
      const int address;
      //const unsigned int deviceId;
};

#endif
