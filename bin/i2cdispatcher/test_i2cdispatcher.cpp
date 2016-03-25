#include "i2cdispatcher.hpp"

int const MOTION_ADDRESS = 0x04;
int const SORTER_ADDRESS = 0x05;

using namespace std;

int main()
{

  unsigned int msg1 = 3000;
  unsigned int msg2 = 389400;
 
  I2cDispatcher i2c_motion(MOTION_ADDRESS);

  i2c_motion.sendData(msg1);

  I2cDispatcher i2c_sorter(SORTER_ADDRESS);

  i2c_sorter.sendData(msg2);

}
