#ifndef ROBOT_HPP
#define ROBOT_HPP

#include <iostream>

#include "gpio.hpp"

#include "i2caddresses.hpp"

#include "cage.hpp"
#include "conveyor.hpp"
#include "sorter.hpp"
#include "grabber.hpp"
#include "colorprocessor.h"
#include "colorprocessor.hpp"
#include "zaix.hpp"
//#include "motion.hpp"

#define ON  1
#define OFF 0

#define SMALL_CAGE_SIZE 2
#define BIG_CAGE_SIZE 4

#define CONV_SLEEP 5000000

const unsigned int MAIN_CONV_DEVICE_ID =  0;
const unsigned int NUM_BARGE_C         =  8;
const unsigned int NUM_BARGE_A         =  4;

class Robot
{
    public:
        //Constructors
        Robot(): cage(MULTI_ADDRESS), mainConv(MULTI_ADDRESS, MAIN_CONV_DEVICE_ID),
                 sorter(SORTER_ADDRESS), grabber(MULTI_ADDRESS) , zaxis(MULTI_ADDRESS)
                  /*, motion(MOVEMENT_ADDRESS, &zaxis)*/, blocksInOneCol(4) {}

        //Public methods
        void waitForStartSwitch();
        void selectCourseFromSwitch();
        void processBarge( int );
        void grabBlock();
        void storeBlock( int );
        void depositTruck();
        void depositBoat();
        void depositHopper( int );

        //Public objects
        Cage cage;
        Conveyor mainConv;
        Sorter sorter;
        Grabber grabber;
        ColorProcessor colorProcessor;
        Gpio gpio;
        //Motion motion;
        Zaxis zaxis;

   private:

        //Private variables
        int blocksInOneCol;

        int largeCageStatus[4];   //keep track of large blocks in cage
        int smallCageStatus[4];   //small blocks

};

#endif
