#include "i2c_setup.hpp"

int const MOTION_ADDRESS = 0x04;
int const SORTER_ADDRESS = 0x05;

using namespace std;

int main()
{
  I2CSetup i2c_motion(MOTION_ADDRESS);

  i2c_motion.sendData(3000);

  I2CSetup i2c_sorter(SORTER_ADDRESS);

  i2c_sorter.sendData(389400);

}
