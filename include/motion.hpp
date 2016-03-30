#ifndef MOTION_HPP
#define MOTION_HPP

#include <iostream>
#include "i2cdispatcher.hpp"
#include "messageformatter.hpp"
#include "coursereader.hpp"
#include "complex.hpp"

#define MOTION_NORTH      0
#define MOTION_SOUTH      1
#define MOTION_WEST       2
#define MOTION_EAST       3
#define MOTION_SPIN_CW    4
#define MOTION_SPIN_ACW   5

#define MOTION_TUNNEL_0  10
#define MOTION_TUNNEL_1  11

using namespace std;

//file names
const string TUNNEL_0 = "courseZeroTunnel.csv";
const string TUNNEL_1 = "courseOneTunnel.csv";

const string COURSE = "course.csv";

class Motion
{
  public:

      //Constructors
      Motion(const int myAddress) : address(myAddress), i2cDispatcher(myAddress),
                                    currentPosition(0,0,0,0) {}

      //public methods
      void setCourse( int );

      void moveThroughTunnel();
      //used by main program
      void next();
      //go to the next block on the barge
      void nextBlock();

      void sendCmd( int , int );

      void moveToCoordinate();

      //public objects
      CourseReader courseCoor;
      CourseReader tunnelCoor;

  private:

     //Private methods
     void processCourse();

     //Private variables
     const int address;
     unsigned int course;

     unsigned int orientation;
     Complex currentPosition;

     //Private objects
     I2cDispatcher i2cDispatcher;;
     MessageFormatter msgFmt;
};

#endif
