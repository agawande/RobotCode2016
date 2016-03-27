#ifndef COLORPROCESSOR_H
#define COLORPROCESSOR_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define COLOR_THRESHOLD 127
#define NUM_COLORS      4

using namespace cv;

//------------ RED HSV -------------
// Hue Limits for red_U
#define RED_UH_UPPER  180
#define RED_UH_LOWER  165

// Saturation Limits for red_U
#define RED_US_UPPER  255
#define RED_US_LOWER  0

// Value Limits for red_U
#define RED_UV_UPPER  255
#define RED_UV_LOWER  0

// Hue Limits for red_L
#define RED_LH_UPPER  10
#define RED_LH_LOWER  0

// Saturation Limits for red_L
// LOWER set to 1 rather than 0 to avoid black
#define RED_LS_UPPER  255
#define RED_LS_LOWER  1

// Value Limits for red_L
// LOWER set to 1 rather than 0 to avoid black
#define RED_LV_UPPER  255
#define RED_LV_LOWER  1


//------------ BLUE HSV -------------
// Hue Limits
#define BLUE_H_UPPER  164
#define BLUE_H_LOWER  91

// Saturation Limits
#define BLUE_S_UPPER  255
#define BLUE_S_LOWER  0

// Value Limits
#define BLUE_V_UPPER  255
#define BLUE_V_LOWER  0

//---------- YELLOW HSV -------------
// Hue Limits
#define YELLOW_H_UPPER  37
#define YELLOW_H_LOWER  11

// Saturation Limits
#define YELLOW_S_UPPER  255
#define YELLOW_S_LOWER  0

// Value Limits
#define YELLOW_V_UPPER  255
#define YELLOW_V_LOWER  0

//------------ GREEN HSV -------------
// Hue Limits
#define GREEN_H_UPPER  90
#define GREEN_H_LOWER  38

// Saturation Limits
#define GREEN_S_UPPER  255
#define GREEN_S_LOWER  0

// Value Limits
#define GREEN_V_UPPER  255
#define GREEN_V_LOWER  0

// Define colors of interest
enum Color
{
   COLOR_RED,
   COLOR_BLUE,
   COLOR_YELLOW,
   COLOR_GREEN,
   COLOR_UNDEFINED
};

#endif
