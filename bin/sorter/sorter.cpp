#include "sorter.hpp"
#include <iostream>

using namespace std;

/**********************************************************************
 ******************** Public Function Definitions ********************
 **********************************************************************/

void Sorter::zAxisTo(int height)
{
    sendCmd(ZAXIS, height);
} //end of zAxisTo

void Sorter::moveToDepositHeight()
{
    sendCmd(DEPOSIT_HEIGHT, 0);
} //end of moveToDepositHeight

void Sorter::sort(int type, int more, int color)
{
    sendCmdRaw(SORT, type, more, color);
} //end of sort

void Sorter::sendCmd(int command, int dist)
{
    i2cDispatcher.sendData(msgFmt.buildMessage(command, dist));
    msgFmt.updateKey();
} //end of senCmd

void Sorter::sendCmdRaw(int command, unsigned int data1, unsigned int data2, unsigned int data3)
{
    i2cDispatcher.sendData(msgFmt.buildMessageRaw(command, data1, data2, data3));
    msgFmt.updateKey();
} //end of sendCmdRaw
