#ifndef MOTION_HPP
#define MOTION_HPP

#include <iostream>
#include "i2cdispatcher.hpp"
#include "messageformatter.hpp"
#include "coursereader.hpp"
#include "coordinate.hpp"
#include "laserprocessor.hpp"
#include "zaxis.hpp"

//Commands
#define MOTION_NORTH      0
#define MOTION_SOUTH      1
#define MOTION_WEST       2
#define MOTION_EAST       3
#define MOTION_SPIN_CW    4
#define MOTION_SPIN_ACW   5
#define MOTION_TUNNEL_0  10
#define MOTION_TUNNEL_1  11

//distances
#define COURSE_LENGTH   2438   //8 feet
#define BARGE_WIDTH     254    //10 inches
#define ALIGN_DIST      318    //12.5 inch
#define INCH            25     //1 inch

using namespace std;

//file names
const string TUNNEL_0 = "courseZeroTunnel.csv";
const string TUNNEL_1 = "courseOneTunnel.csv";

const string COURSE = "course.csv";

class Motion
{
  public:

      //Constructors
      Motion(const int myAddress, Zaxis &zaxis) : address(myAddress), i2cDispatcher(myAddress),
                                                  zaxis(zaxis), currentPosition(0,0,0,0) {}

      //public methods
      void setCourse( int );

      void moveThroughTunnel();
      void next();
      //go to the next block on the barge
      void nextBlock( int shouldAlignToBlock );

      void sendCmd( int , int );

      //public objects
      CourseReader courseCoor;
      CourseReader tunnelCoor;

      LaserProcessor laserProcessor;
      Zaxis &zaxis;

  private:

     //Private methods
     void processCourse();
     void moveToCoordinate();
     void inFrontOfNextBlock(int direction);

     //Private variables
     const int address;
     unsigned int course;

     unsigned int orientation;
     Coordinate currentPosition;

     //Private objects
     I2cDispatcher i2cDispatcher;;
     MessageFormatter msgFmt;
};

#endif
