#ifndef SORTER_HPP
#define SORTER_HPP

#include <iostream>
#include "i2cdispatcher.hpp"
#include "messageformatter.hpp"

using namespace std;

#define DEPLOY_GRABBER  0
#define ZAXIS           1
#define DEPOSIT_HEIGHT  2
#define SORT            3

#define ENV_HEIGHT       0
#define LOW_LOAD_HEIGHT  1
#define MID_LOAD_HEIGHT  2
#define HIGH_LOAD_HEIGHT 3

#define SORTER_RED     0
#define SORTER_YELLOW  1
#define SORTER_GREEN   2
#define SORTER_BLUE    3

#define SORTER_BIG   0
#define SORTER_SMALL 1

class Sorter
{
  public:

      //Constructors
      Sorter(const int myAddress) : address(myAddress), i2cDispatcher(address) {}

      //public methods
      void sort( int, int, int );
      void deployGrabber();

      void sendCmd( int , int );
      void sendCmdRaw( int , unsigned int , unsigned int , unsigned int );

  private:

     //Private variables
     const int address;

     //Private objects
     I2cDispatcher i2cDispatcher;;
     MessageFormatter msgFmt;
};

#endif
