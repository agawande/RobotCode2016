#ifndef LASERPROCESSOR_H
#define LASERPROCESSOR_H

#include <stdio.h>
#include <time.h>
#include <opencv2/core/core.hpp>

using namespace cv;


/*****************************************************************************
 *                      LASER MASK HSV RANGES                                *
 *                                                                           *
 *  NOTE:                                                                    *
 *   1) Using a red laser so must use Upper & Lower Ranges                   *
 *   2) Laser has high intensity (i.e. ~white) at brighter or closer objects *
 *   3) Must bitwise_or() upper and lower ranges as well as high intensity   *
 *****************************************************************************/

//------------ RED LASER HSV -------------
// Hue Limits for Upper Range
const unsigned int LASER_RED_UH_MAX = 180;
const unsigned int LASER_RED_UH_MIN = 165;

// Saturation Limits for Upper Range
const unsigned int LASER_RED_US_MAX = 255;
const unsigned int LASER_RED_US_MIN = 0;

// Value Limits for Upper Range
const unsigned int LASER_RED_UV_MAX = 255;
const unsigned int LASER_RED_UV_MIN = 178;

// Hue Limits for Lower Range
const unsigned int LASER_RED_LH_MAX = 10;
const unsigned int LASER_RED_LH_MIN = 0;

// Saturation Limits for Lower Range
const unsigned int LASER_RED_LS_MAX = 255;
const unsigned int LASER_RED_LS_MIN = 178;

// Value Limits for Lower Range
const unsigned int LASER_RED_LV_MAX = 255;
const unsigned int LASER_RED_LV_MIN = 178;

//------------ BRIGHT LASER HSV ----------
// Hue Limits 
const unsigned int LASER_BRIGHT_H_MAX = 180;
const unsigned int LASER_BRIGHT_H_MIN = 0;

// Saturation Limits 
const unsigned int LASER_BRIGHT_S_MAX = 255;
const unsigned int LASER_BRIGHT_S_MIN = 0;

// Value Limits 
const unsigned int LASER_BRIGHT_V_MAX = 255;
const unsigned int LASER_BRIGHT_V_MIN = 254;



/**************************************************************
 *                        General Constants                   *
 **************************************************************/

//Region of interest - origin is upper left of image - keep as small as possible
const unsigned int ROI_X      =  190; //Left Side of Conveyor, closest to camera
const unsigned int ROI_Y      =  240; //Needs to be higher than (height of image)/2
const unsigned int ROI_WIDTH  =  300; //(Right side of conveyor)-ROI_X
const unsigned int ROI_HEIGHT =  240; //(height of image)-ROI_Y

const unsigned int BLACK = 0;   //based on inRange() output
const unsigned int WHITE = 255; //based on inRange() output



/**************************************************************
 *                    findLaser Constants                     *
 **************************************************************/

//These parameters are used to clean up the laser mask (should result in thicker line)
const unsigned int FILTER_KERNAL_SIZE = 9; // ODD(3 to 15), don't make too big (i.e. <= 15)
const unsigned int FILTER_THRESHOLD   = 64; // 0-255, Less than this value is black, otherwise white


/**************************************************************
 *               laserProximityCheck Constants                *
 **************************************************************/

//Lookup Table
extern const int distanceLookup[200];


//Functionality
const unsigned int PROXIMITY_ALLOWANCE = 0.05; //Allowance for proximity check i.e. 0.0 - 1.0
const unsigned int PROXIMITY_RANGE     = 5; //Will give us a little spread, in #pixels
const unsigned int PROXIMITY_HEIGHT    = 50;

/**************************************************************
 *           laserPerpendicularityCheck Constants             *
 **************************************************************/

const unsigned int PI = 3.14159265;

#endif
