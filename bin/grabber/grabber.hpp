#ifndef GRABBER_HPP
#define GRABBER_HPP

#include "conveyor.hpp"
#include "clamper.hpp"

const unsigned int GRABBER_CONVEYER_DEVICE_ID = 5;
const unsigned int GRABBER_CLAMPER_DEVICE_ID  = 8;

class Grabber
{
   public:

      //Constructors
      Grabber(const int myAddress) : grabberConveyer(myAddress, GRABBER_CONVEYER_DEVICE_ID),
                                     clamper(myAddress, GRABBER_CLAMPER_DEVICE_ID) {}

      //Grabber Conveyor
      Conveyor grabberConveyer;
      Clamper clamper;
};

#endif
