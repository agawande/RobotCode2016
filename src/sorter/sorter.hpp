using namespace std;

int const SORTER_ADDRESS = 0x03;

#define HOME 0
#define D1 1
#define D2 2
#define D3 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7
#define D8 8
#define PICKUP 9

class Sorter
{
public:
  Sorter()
    : i2c_sorter(SORTER_ADDRESS)
    , msgFmt()
    {
    }

  int
  checksorterState()
  {
    char buf[1];
    if (read(i2c_sorter.getFile(), buf, 1) == 1){
       sorterState = (int) buf[0];
    }
    return sorterState;
  }

  void
  checkForCompletion()
  {
    while(1)
    {
      //Read the bus, it should be a 1 or a 0.
      char buf[1];
      if (read(i2c_sorter.getFile(), buf, 1) == 1){
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

  void goTo(string color, int lsw_middle)
  {
    switch(lsw_middle)
    {
      case 0:
      case 1:
    }
  }

  void
  goToSmallBlue()
  {
    goToD1();
  }

  void
  goToSmallGreen()
  {
    goToD2();
  }

  void
  goToSmallYellow()
  {
    goToD3();
  }

  void
  goToSmallRed()
  {
    goToD4();
  }

  void
  goToBigBlue()
  {
    goToD5();
  }

  void
  goToBigGreen()
  {
    goToD6();
  }

  void
  goToBigYellow()
  {
    goToD7();
  }

  void
  goToBigRed()
  {
    goToD8();
  }

  void
  goToPickUp()
  {
    sendCmd(PICKUP);
  }

  void
  goToD1()
  {
    sendCmd(D1);
  }

  void
  goToD2()
  {
    sendCmd(D2);
  }

  void
  goToD3()
  {
    sendCmd(D3);
  }

  void
  goToD4()
  {
    sendCmd(D4);
  }

  void
  goToD5()
  {
    sendCmd(D5);
  }

  void
  goToD6()
  {
    sendCmd(D6);
  }

  void
  goToD7()
  {
    sendCmd(D7);
  }

  void
  goToD8()
  {
    sendCmd(D8);
  }

  void
  dropGrabber()
  {

  }

  void
  sendCmd(int command){
    i2c_sorter.sendData(msgFmt.buildMessage(command, 0));
    msgFmt.updateKey();
  }

private:
  I2CSetup i2c_sorter;
  MessageFormatter msgFmt;
  int sorterState;
};
