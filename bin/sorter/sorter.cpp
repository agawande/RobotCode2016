#include "sorter.hpp"
#include <iostream>

using namespace std;

/**********************************************************************
 ******************** Public Function Definitions ********************
 **********************************************************************/

void Sorter::sort(int type, int more, int color)
{
    sendCmdRaw(SORT, type, more, color);
} //end of sort

void Sorter::deployGrabber()
{
    //Don't care about the distance
    sendCmd(DEPLOY_GRABBER, 0);
} //end of deploy grabber

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
