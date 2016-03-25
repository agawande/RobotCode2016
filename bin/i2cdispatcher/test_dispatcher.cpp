#include "../communication/i2c_setup.hpp"
#include "../communication/message_formatter.hpp"
#include "multi_util.hpp"

using namespace std;

int main()
{
  MultiUtil multi;

  multi.startConveyerI();

  usleep(5000000);

  multi.stopConveyerI();

  multi.startConveyerII();

  usleep(5000000);

  multi.stopConveyerII();

  multi.startConveyerIII();

  usleep(5000000);

  multi.stopConveyerIII();

  multi.startConveyerIV();

  usleep(5000000);

  multi.stopConveyerIV();
}
