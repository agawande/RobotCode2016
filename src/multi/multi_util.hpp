using namespace std;

int const MULTI_ADDRESS = 0x05;

#define CAGE_CONVEYER_1_ON      0
#define CAGE_CONVEYER_1_OFF    1
#define CAGE_CONVEYER_2_ON      2
#define CAGE_CONVEYER_2_OFF    3
#define CAGE_CONVEYER_3_ON      4
#define CAGE_CONVEYER_3_OFF    5
#define CAGE_CONVEYER_4_ON      6
#define CAGE_CONVEYER_4_OFF    7

#define MAIN_CONVEYER_ON           8
#define MAIN_CONVEYER_OFF         9

#define GRABBER_CONVEYER_ON   10
#define GRABBER_CONVEYER_OFF  11

#define CLAMPER_CLOSE                  12
#define CLAMPER_OPEN                    13

#define COUPLE                                  14
#define DECOUPLE                              15

class MultiUtil
{
public:
  MultiUtil()
    : i2c_sorter(MULTI_ADDRESS)
    , msgFmt()
    {
    }
	
private:
  void
  sendCmd(int command){
    i2c_sorter.sendData(msgFmt.buildMessage(command, 0));
    msgFmt.updateKey();
  }

private:
  I2CSetup i2c_sorter;
  MessageFormatter msgFmt;
};