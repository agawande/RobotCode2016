#include <iostream>
#include "cage.hpp"
#include "i2caddresses.hpp"

int main()
{
   Cage cage(MULTI_ADDRESS);

   cage.redConveyor.moveForward();
   cage.redConveyor.stop();

   cage.coupling.couple();
   cage.coupling.decouple();
 
   return 1;
}
