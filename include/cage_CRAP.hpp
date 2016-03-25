using namespace std;

int const MULTI_ADDRESS = 0x05;

#define MAIN_CONVEYER_ON       8
#define MAIN_CONVEYER_OFF      9

#define GRABBER_CONVEYER_ON   10
#define GRABBER_CONVEYER_OFF  11

#define CLAMPER_CLOSE         12
#define CLAMPER_OPEN          13

#define COUPLE                14
#define DECOUPLE              15

//Using distance for extra commands
#define SORTER_GRABBER_PICK  1
#define SORTER_GRABBER_DROP  2

class Cage
{
   public:

      //Constructors
      Cage()

    : i2c_multi(MULTI_ADDRESS)
    , msgFmt()
    {
    }

  void
  startMainConveyer()
  {
    sendCmd(MAIN_CONVEYER_ON);
  }

  void
  stopMainConveyer()
  {
    sendCmd(MAIN_CONVEYER_OFF);
  }

  void
  sorterPickUp()
  {
    sendDistCmd(SORTER_GRABBER_PICK);
  }

  void
  sorterDrop()
  {
    sendDistCmd(SORTER_GRABBER_DROP);
  }

  void
  startGrabberConveyer()
  {
    sendCmd(GRABBER_CONVEYER_ON);
  }

  void
  stopGrabberConveyer()
  {
    sendCmd(GRABBER_CONVEYER_OFF);
  }

  void
  clampGrabber()
  {
    sendCmd(CLAMPER_CLOSE);
  }

  void
  deClampGrabber()
  {
    sendCmd(CLAMPER_OPEN);
  }

  void
  startConveyerI()
  {
    sendCmd(CAGE_CONVEYER_1_ON);
  }

  void
  stopConveyerI()
  {
    sendCmd(CAGE_CONVEYER_1_OFF);
  }

  void
  startConveyerII()
  {
    sendCmd(CAGE_CONVEYER_2_ON);
  }

  void
  stopConveyerII()
  {
    sendCmd(CAGE_CONVEYER_2_OFF);
  }

  void
  startConveyerIII()
  {
    sendCmd(CAGE_CONVEYER_3_ON);
  }

  void
  stopConveyerIII()
  {
    sendCmd(CAGE_CONVEYER_3_OFF);
  }

  void
  startConveyerIV()
  {
    sendCmd(CAGE_CONVEYER_4_ON);
  }

  void
  stopConveyerIV()
  {
    sendCmd(CAGE_CONVEYER_4_OFF);
  }

private:
  void
  sendDistCmd(unsigned int dist){
    if(msgFmt.checkData(dist) == GOOD){
      if(dist == 1 || dist == 2) {
        i2c_multi.sendData(msgFmt.buildMessage(0, dist));
        msgFmt.updateKey();
      }
    }
  }

  void
  sendCmd(int command){
    i2c_multi.sendData(msgFmt.buildMessage(command, 0));
    msgFmt.updateKey();
  }

private:
  I2CSetup i2c_multi;
  MessageFormatter msgFmt;
};
