#include "../communication/i2c_setup.hpp"
#include "../communication/message_formatter.hpp"
#include "../motion/motion.hpp"
#include "../sorter/sorter.hpp"
#include "../multi/multi_util.hpp"

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
  //Grabber grabber;
  //Conveyer conveyer;
  MultiUtil multi;

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

  //Open the grabber, ready to grab
  multi.deClampGrabber();

  //motion.moveNext(courseFile.next(), );

  //moveInFrontOfBargeBBlock1();

  //This should work for any barge, will change the name once I make sure
  takeInBargeB();

  return 0;
}

void storeBlock()
{
  string color = idBlockColor.readColor();

  //Go above the block
  sorter.goToPickUp();
  //actually grab the block
  multi.sorterPickUp();
  sorter.goTo(color, digitalRead(LSW_MIDDLE));
  multi.sorterDrop();
}

void grabSet()
{
   //Clamp the grabber around block
   multi.clampGrabber();

   //Start the grabber's conveyer
   multi.startGrabberConveyer();
}

void takeInBargeB()
{
  sorter.toHeightBargeB();
  grabSet();  //grab one column

  while(digitalRead(LSW_CORNER) == OFF && digitalRead(LSW_MIDDLE) == OFF) {
    multi.startMainConveyer();    //if it is already running?
  }
  multi.stopMainConveyer();
  multi.stopGrabberConveyer();

  storeBlock();

  if (digitalRead(LSW_MIDDLE) == ON) {
    //A small block was picked up, 3 blocks left or 1 small block and 1 large block left
    while(digitalRead(LSW_CORNER) == OFF && digitalRead(LSW_CORNER) == OFF) {
      multi.startMainConveyer();
    }
    multi.stopMainConveyer();

    storeBlock();

    while(digitalRead(LSW_CORNER) == OFF) {
      multi.startMainConveyer();
    }
    multi.stopMainConveyer();

    storeBlock();

    if (digitalRead(LSW_MIDDLE) == ON) {
       //1 small block left, else nothing left
       while(digitalRead(LSW_CORNER) == OFF) {
         multi.startMainConveyer();
       }
       multi.stopMainConveyer();

       storeBlock();
    }

  } else {
    //A large block was picked up
    // Now either two small blocks are left or 1 big block
    while(digitalRead(LSW_CORNER) == OFF && digitalRead(LSW_MIDDLE) == OFF) {
      multi.startMainConveyer();    //if it is already running?
    }
    multi.stopMainConveyer();

    storeBlock();

    if (digitalRead(LSW_MIDDLE) == ON) {
       //1 small block left, else nothing left
       while(digitalRead(LSW_CORNER) == OFF) {
         multi.startMainConveyer();
       }
       multi.stopMainConveyer();

       storeBlock();
    }
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
