#ifndef ZAIXS_HPP
#define ZAXIS_HPP

#include <iostream>
#include "i2cdispatcher.hpp"
#include "messageformatter.hpp"

using namespace std;


#define ZAXIS           1
#define DEPOSIT_HEIGHT  2

#define ENV_HEIGHT       0
#define LOW_LOAD_HEIGHT  1
#define MID_LOAD_HEIGHT  2
#define HIGH_LOAD_HEIGHT 3

class Zaxis
{
  public:

      //Constructors
      Zaxis(const int myAddress) : address(myAddress), i2cDispatcher(address) {}

      //public methods
      void toHeight( int );
      void moveToDepositHeight();

      void sendCmd(int, int);

  private:

     //Private variables
     const int address;

     //Private objects
     I2cDispatcher i2cDispatcher;;
     MessageFormatter msgFmt;
};

#endif
