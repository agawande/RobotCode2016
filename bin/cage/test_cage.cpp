#include <iostream>
#include "cage.hpp"
#include "i2caddresses.hpp"
#include <unistd.h>

int main()
{
   //Cage cage(MULTI_ADDRESS);
   Cage cage(0x05);

   cage.redConveyor.moveForward();
   usleep(10000000);
   cage.redConveyor.stop();
/*
   cage.coupling.couple();
   cage.coupling.decouple();

   return 1;*/
}
