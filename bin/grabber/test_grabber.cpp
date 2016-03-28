#include "grabber.hpp"

int main()
{
  int address = 0x05;

  Grabber grabber(address);

  grabber.conveyor.moveForward();

  grabber.clamper.close();
}
