#include "zaxis.hpp"

void Zaxis::toHeight(int height)
{
    sendCmd(ZAXIS, height);
} //end of zAxisTo

void Zaxis::moveToDepositHeight()
{
    //Don't care bout distance
    sendCmd(DEPOSIT_HEIGHT, 0);
} //end of moveToDepositHeight

void Zaxis::sendCmd(int command, int dist)
{
    i2cDispatcher.sendData(msgFmt.buildMessage(command, dist));
    msgFmt.updateKey();
} //end of sendCmd

