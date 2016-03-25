#include "grabber.hpp"

int main()
{
  int address = 0x05;

  Grabber grabber(address);

  grabber.grabberConveyer.moveForward();

  grabber.clamper.close();
}
