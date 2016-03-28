#include "messageformatter.hpp"
#include "conveyor.hpp"

using namespace std;

int main()
{

  unsigned int deviceId = 4;
  const int address = 0x05;

  Conveyor blue(address, deviceId);

  blue.moveForward();

}
