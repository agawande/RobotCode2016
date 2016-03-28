#include "sorter.hpp"
#include "cage.hpp"
#include "grabber.hpp"
#include "i2caddresses.hpp"
//#include "robot.hpp"

#include <unistd.h>
#include <iostream>
#include <ctime>

using namespace std;

int main()
{
  unsigned int microseconds = 10000000;

  Conveyor mainConv(MULTI_ADDRESS, 0);

  //Start main conveyor
  //mainConv.moveForward();
  //usleep(microseconds/3.0);

  Grabber grabber(MULTI_ADDRESS);

  //Start grabber conveyor
  //grabber.conveyer.moveForward();

/*
  clock_t start = clock();

  //Flop for 5 seconds
  while( ( clock()-start )/( (double)(CLOCKS_PER_SEC) ) < 5) {
     grabber.clamper.open();
     //sleep for 1 second
     usleep(1000000);
     grabber.clamper.close();
  }
*/
  grabber.clamper.open();
  usleep(microseconds/2);
  grabber.clamper.close();
  //Stop grabber conveyor
  //grabber.conveyer.stop();

  //Stop main conveyor
  //mainConv.stop();

  /*Sorter sorter(SORTER_ADDRESS);

  sorter.moveToDepositHeight();

  usleep(2*microseconds);

  Cage cage(0x05);

  cage.redConveyor.moveForward();
  cage.blueConveyor.moveForward();
  cage.yellowConveyor.moveForward();
  cage.greenConveyor.moveForward();
  usleep(microseconds);
  cage.redConveyor.stop();
  cage.blueConveyor.stop();
  cage.yellowConveyor.stop();
  cage.greenConveyor.stop();

/*  cage.blueConveyor.moveForward();
  usleep(microseconds);
  cage.blueConveyor.stop();

  cage.yellowConveyor.moveForward();
  usleep(microseconds);
  cage.yellowConveyor.stop();

  cage.greenConveyor.moveForward();
  usleep(microseconds);
  cage.greenConveyor.stop();

/*
  sorter.sort(BIG, 1, BLUE);

  usleep(microseconds);

  sorter.sort(BIG, 1, YELLOW);

  usleep(microseconds);

  sorter.sort(BIG, 1, RED);

  usleep(microseconds);

  sorter.sort(BIG, 1, GREEN);*/
}
