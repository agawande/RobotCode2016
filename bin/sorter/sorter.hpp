#include <iostream>
#include "i2caddresses.hpp"

using namespace std;

int const SORTER_ADDRESS = 0x03;

#define DEPLOY_GRABBER  0
#define ZAXIS           1
#define DEPOSIT_HEIGHT  2
#define SORT            3

#define ENV_HEIGHT       0
#define LOW_LOAD_HEIGHT  1
#define MID_LOAD_HEIGHT  2
#define HIGH_LOAD_HEIGHT 3

#define RED     0
#define YELLOW  1
#define GREEN   2
#define BLUE    3

#define BIG   0
#define SMALL 1

class Sorter {
   public:
      Sorter() : i2c_sorter(SORTER_ADDRESS) , msgFmt() {}

      int checksorterState() 
      {
         char buf[1];
         if (read(i2c_sorter.getFile(), buf, 1) == 1){
         sorterState = (int) buf[0];
      }

         return sorterState;
      }

   void checkForCompletion() 
   {
      while(1) { 

      //Read the bus, it should be a 1 or a 0.
      char buf[1];

      if (read(i2c_sorter.getFile(), buf, 1) == 1)
      {
         //store the byte into sorterState
         sorterState = (int) buf[0];

         //If 0 is return, Teensy finished tasked, and the loop
         //can be broken out of, to continue with the process.
         if (sorterState == 0)
         {
            //cosmetic stuff.
            std::cout << "Received Completion Response" << endl;
            break;
         }
      }
   }
   //ensure sorterState is 1 again
   sorterState = 1;
   }

  void
  zAxisTo(int height) {
    sendCmd(ZAXIS, height);
  }

  void
  moveToDepositHeight() {
    sendCmd(DEPOSIT_HEIGHT, 0);
  }

  void
  sort(int type, int more, int color) {
    sendCmdRaw(SORT, type, more, color);
  }

  void
  sendCmd(int command, int dist){
    i2c_sorter.sendData(msgFmt.buildMessage(command, dist));
    msgFmt.updateKey();
  }

  void
  sendCmdRaw(int command, unsigned int data1, unsigned int data2, unsigned int data3) {
    i2c_sorter.sendData(msgFmt.buildMessageRaw(command, data1, data2, data3));
    msgFmt.updateKey();
  }

private:
  I2CSetup i2c_sorter;
  MessageFormatter msgFmt;
  int sorterState;
};
