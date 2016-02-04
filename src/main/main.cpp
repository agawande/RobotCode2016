#include "../communication/i2c_setup.hpp"
#include "../communication/message_formatter.hpp"
#include "../motion/motion.hpp"
#include "../sorter/sorter.hpp"

#include "../vision/color/id_block_color.hpp"
#include "../vision/color/gpio.h"

#define BARGE_A_COLS 5
#define BARGE_B_COLS 8
#define BARGE_C_COLS 8

using namespace std;

void waitForGoSwitch()
{
  while (digitalRead(GO_SWITCH) == OFF)
  {
    cout << "Waiting for go switch..." << endl;
    if(digitalRead(GO_SWITCH) == ON) {
      cout << "The game is afoot!" << endl;
      break;
    }
    sleep(1);
  }
}

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

  waitForGoSwitch();

  motion.crossTunnel();
  //will have to wait so that we are out of the tunnel
  motion.checkForCompletion();

  //Drop the grabber once we are out of the tunnel
  sorter.dropGrabber();

  //Get the distance from front
  //double distFromFront = laser.distanceFromFront();

  //grabber.getLength() would have a slightly overestimated length so that we don't bump into the target
  //motion.moveForward(distFromFront - grabber.getLength());

  //double distFromShoterSide = laser.distanceFromShorterSide();

  //motion.moveNext(courseFile.next(), );

  //maybe need to pass motion to it
  //laser.alignToObject();

  moveInFrontOfBargeBBlock1();

  takeInBargeB();
  grabber.grabSet();

  return 0;
}

void takeInBargeB()
{
  lift.toHeightB();
  grabber.grabSet();  //grab two columns

  while(LSW_CORNER == OFF) {
    conveyer.run();    //if it is already running, this method won't do anything
  }
  conveyer.stop();

  string color = idBlockColor.readColor();
  sorter.pickUp();
  if (LSW_MIDDLE == OFF) {
    sorter.drop(color);
  } else {
    sorter.drop(color);
  }
}


/*
    D1 D2 D3 D4: Small sized block
     B  G  Y  R
    D5 D6 D7 D8: Large sized block

    BACK
    -----------------
    | D5 D6 D7 D8 | |
    | B  G  Y  R  | |
    | B  G  Y  R  | |
    | D1 D2 D3 D4 | |
    | b  g  y  r  | |
    ------------------
    FRONT
*/
