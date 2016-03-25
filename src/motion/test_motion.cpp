#include "../communication/i2c_setup.hpp"
#include "../communication/message_formatter.hpp"
#include "motion.hpp"

using namespace std;

int main()
{
  Motion motion;

  //motion.moveRight(1000);

  //usleep(5000000);

  motion.moveForward(1000);
  motion.moveForward(200);

  //usleep(5000000);

  //motion.spinACW(90);

  //usleep(5000000);

  //motion.spinCW(90);

  //motion.moveQ2(1000);

  //motion.moveQ1(500);
  //usleep(5000000);
  //motion.moveQ2(500);
  //usleep(5000000);
  //motion.moveQ3(500);
  //usleep(5000000);
  //motion.moveQ4(500);
  //usleep(5000000);
}
