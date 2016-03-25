#ifndef SORTER_HPP
#define CONVEYER_HPP

#include <iostream>
#include "i2cdispatcher.hpp"
#include "messageformatter.hpp"

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

class Sorter
{
  public:

      //Constructors
      Sorter() : i2cDispatcher(SORTER_ADDRESS) {}

      //public methods
      void zAxisTo( int );
      void moveToDepositHeight();
      void sort( int, int, int );
      void sendCmd( int , int );
      void sendCmdRaw( int , unsigned int , unsigned int , unsigned int );

  private:

     //Private objects
     I2cDispatcher i2cDispatcher;;
     MessageFormatter msgFmt;
};

#endif
