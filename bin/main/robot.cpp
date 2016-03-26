#include "conveyor.hpp"
//#include "sorter.hpp"
//#include "grabber.hpp"
#include "i2caddresses.hpp"
#include <unistd.h>

using namespace std;

int main()
{
  Conveyor mainConv(MULTI_ADDRESS, 0);

  mainConv.moveForward();

  while(1) {
     usleep(100000);
  }

  //Grabber grabber();
}
