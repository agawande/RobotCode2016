#include "clamper.hpp"

int main()
{
  unsigned int deviceID = 8;
  int address = 0x05;

  Clamper clamper(address, deviceID);
  clamper.open();
}
