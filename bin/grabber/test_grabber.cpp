#include "grabber.hpp"
#include <ctime>
#include <unistd.h>

using namespace std;

int main()
{
  int address = 0x05;

  Grabber grabber(address);

  grabber.conveyor.moveForward();

  grabber.clamper.close();

  clock_t start;
  start = clock();

  //run for 5 seconds
  while((clock() - start)/((double)(CLOCKS_PER_SEC)) < 5) {
      if(grabber.clamper.getState() == CLAMPER_OPEN) {
           grabber.clamper.close();
      } else {
         grabber.clamper.open();
      }
      usleep(1000000);
  }

  grabber.conveyor.stop();
}
