using namespace std;

int const MULTI_ADDRESS = 0x05;

#define CAGE_CONVEYER_1_ON     0
#define CAGE_CONVEYER_1_OFF    1
#define CAGE_CONVEYER_2_ON     2
#define CAGE_CONVEYER_2_OFF    3
#define CAGE_CONVEYER_3_ON     4
#define CAGE_CONVEYER_3_OFF    5
#define CAGE_CONVEYER_4_ON     6
#define CAGE_CONVEYER_4_OFF    7

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

class MultiUtil
{
public:
  MultiUtil()
    : i2c_sorter(MULTI_ADDRESS)
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

private:
  void
  sendDistCmd(unsigned int dist){
    if(msgF.checkData(dist) == GOOD){
      if(dist == 1 || dist == 2) {
        i2c_motion.sendData(msgF.buildMessage(command, dist));
        msgF.updateKey();
      }
    }
  }

  void
  sendCmd(int command){
    i2c_sorter.sendData(msgFmt.buildMessage(command, 0));
    msgFmt.updateKey();
  }

private:
  I2CSetup i2c_sorter;
  MessageFormatter msgFmt;
};
