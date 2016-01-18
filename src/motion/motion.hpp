#include "../communication/i2c_setup.hpp"
#include "../communication/message_formatter.hpp"

using namespace std;

int const MOTION_ADDRESS = 0x04;

// Command constants
#define FORWARD         0
#define REVERSE         1
#define PIVOT_RIGHT     2
#define PIVOT_LEFT      3
#define CW_SPIN         4
#define ACW_SPIN        5
#define MOVE_QI         6
#define MOVE_QII        7
#define MOVE_QIII       8
#define MOVE_QIV        9

class Motion
{
public:
  Motion()
    : i2c_motion(MOTION_ADDRESS)
    , msgF()
    {
    }

  //function used to determine the motor state, reads from the Teensy via i2c,
  //and either returns a 1 or a 0 which the Teensy provides
  int
  checkMotorState()
  {
    char buf[1];
    if (read(i2c_motion.getFile(), buf, 1) == 1){
       motorState = (int) buf[0];
    }
    return motorState;
  }

  //SECOND NESTED WHILE LOOP* is used to check for the completion of the command sent
  //to the Teensy 3.1
  void
  checkForCompletion()
  {
    while(1)
    {
      //Read the bus, it should be a 1 or a 0.
      char buf[1];
      if (read(i2c_motion.getFile(), buf, 1) == 1){
        //store the byte into motorState
        motorState = (int) buf[0];
        //If 0 is return, Teensy finished tasked, and the loop
        //can be broken out of, to continue with the process.
        if (motorState == 0){
          //cosmetic stuff.
          std::cout << "Received Completion Response" << endl;
          break;
        }
      }
    }
    //ensure motorState is 1 again
    motorState = 1;
  }


  void
  moveForward(unsigned int dist)
  {
    sendCmd(dist, FORWARD);
  }

  void
  moveReverse(unsigned int dist)
  {
    sendCmd(dist, REVERSE);
  }

  void
  moveLeft(unsigned int dist)
  {
    sendCmd(dist, PIVOT_LEFT);
  }

  void
  moveRight(unsigned int dist)
  {
    sendCmd(dist, PIVOT_RIGHT);
  }

  void
  spinCW(unsigned int dist)
  {
    sendCmd(dist, CW_SPIN);
  }

  void
  spinACW(unsigned int dist)
  {
    sendCmd(dist, ACW_SPIN);
  }

  void
  moveQ1(unsigned int dist)
  {
    sendCmd(dist, MOVE_QI);
  }

  void
  moveQ2(unsigned int dist)
  {
    sendCmd(dist, MOVE_QII);
  }

  void
  moveQ3(unsigned int dist)
  {
    sendCmd(dist, MOVE_QIII);
  }

  void
  moveQ4(unsigned int dist)
  {
    sendCmd(dist, MOVE_QIV);
  }


  void
  exitTunnel()
  {

  }

private:
  void
  sendCmd(unsigned int dist, int command){
    if(msgF.checkData(dist) == GOOD){
      i2c_motion.sendData(msgF.buildMessage(command, dist));
      msgF.updateKey();
    }
  }

private:
  I2CSetup i2c_motion;
  MessageFormatter msgF;
  int motorState;
};
