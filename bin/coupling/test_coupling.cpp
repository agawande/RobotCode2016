#include "coupling.hpp"

using namespace std;

int main()
{
  const int address = 0x05;
  const unsigned int deviceId = 1;

  Coupling cageCoupler(address, deviceId);

  cageCoupler.couple();
  cageCoupler.decouple();

  return 0;

}
