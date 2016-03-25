//using namespace std;

int const MULTI_ADDRESS = 0x05;

#define MAIN 0
#define BLUE 1
#define GREEN 2
#define YELLOW 3
#define RED 4
#define GRABBER_CONV 5
#define COUPLER 6
#define SORTER 7

#define OFF      0
#define FWD      1
#define REV      2
#define SHUFFLE  3

#define COUPLE   1
#define DECOUPLE 0

//#include <iostream>

class MultiUtil
{
public:
  MultiUtil()
    : i2c_multi(MULTI_ADDRESS)
    , msgFmt()
    {
    }

  void
  controlConv(int device, int function)
  {
    sendCmd(device, function);
  }

  void
  couple()
  {
    sendCmd(COUPLER, COUPLE);
  }

  void
  deCouple()
  {
    sendCmd(COUPLER, DECOUPLE);
  }

  //void
  //sorter

  void
  checkForCompletion()
  {
    while(1)
    {
      //Read the bus, it should be a 1 or a 0.
      char buf[1];
      if (read(i2c_multi.getFile(), buf, 1) == 1){
        //store the byte into sorterState
        sorterState = (int) buf[0];
        //If 0 is return, Teensy finished tasked, and the loop
        //can be broken out of, to continue with the process.
        if (sorterState == 0){
          //cosmetic stuff.
          std::cout << "Received Completion Response" << endl;
          break;
        }
      }
    }
    //ensure sorterState is 1 again
    sorterState = 1;
  }


private:
  void
  sendDistCmd(unsigned int dist){
    if(msgFmt.checkData(dist) == GOOD){
      if(dist == 1 || dist == 2) {
        i2c_multi.sendData(msgFmt.buildMessage(0, dist));
        msgFmt.updateKey();
        //cout << "key: " << msgFmt.getKey() << endl;
      }
    }
  }

  void
  sendCmd(int command, unsigned int dist){
      i2c_multi.sendData(msgFmt.buildMessage(command, dist));
      msgFmt.updateKey();
  }

private:
  I2CSetup i2c_multi;
  MessageFormatter msgFmt;
  int sorterState;
};
