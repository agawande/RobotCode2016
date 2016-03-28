#include "robot.hpp"

int main()
{
  Robot robot;

  //Select and set course
  robot.selectCourseFromSwitch();

  //Wait for a loop for the switch to turn on
  robot.waitForStartSwitch();

  //Move thourgh the tunnel
  //motion.moveThroughTunnel();

  //After moving throught the tunnel, deploy grabber
  robot.sorter.deployGrabber();

  //Move infront of the first column of the block (BARGE C)
  //motion.next();

  //Grab and sort Barge C
  robot.processBarge(NUM_BARGE_C);

  //Move to deposit location (truck)
  //motion.next();

  //Deposit blocks in truck
  //robot.truckDeposit();
}
