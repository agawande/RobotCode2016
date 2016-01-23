#include "../motion/motion.hpp"
#include "../sorter/sorter.hpp"

#include "../vision/color/id_block_color.hpp"
#include "../vision/color/gpio.h"

using namespace std;

int main()
{
  wiringPiSetupGpio();

  Motion motion;
  Sorter sorter;
  //Laser laser;
  //Coupler coupler;  //will start as decoupled in the setup
  //Lift lift;
  //Grabber grabber;
  //Conveyer conveyer;

  IDBlockColor idBlockColor;

  if (digitalRead(COURSE_SWITCH) == ON) {
    //Course 1
    CourseFile courseFile("version_1");
  } else {
    //Course 2
    CourseFile courseFile("version_2");
  }

  motion.crossTunnel();
  //will have to wait so that we are out of the tunnel
  motion.checkForCompletion();

  //Drop the grabber once we are out of the tunnel
  sorter.dropGrabber();

  //Get the distance from front
  double distFromFront = laser.distanceFromFront();

  //grabber.getLength() would have a slightly overestimated length so that we don't bump into the target
  motion.moveForward(distFromFront - grabber.getLength());

  double distFromShoterSide = laser.distanceFromShorterSide();

  motion.moveNext(courseFile.next(), );

  //maybe need to pass motion to it
  laser.alignToObject();

  grabber.grabSet();

  //Limit switches

//distanceinfront, distance to side, perpendicular to an object
//5-6 inches extrude - grabber
}
