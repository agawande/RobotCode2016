#ifndef COLOR_H
#define COLOR_H

#include <stdio.h>
#include <time.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define COLOR_THRESHOLD 127
#define VALUE_THRESHOLD 100

using namespace cv;
using namespace std;

//------------ RED HSV -------------
// Hue Limits for red_U
#define RED_UH_UPPER  180
#define RED_UH_LOWER  165

// Saturation Limits for red_U
#define RED_US_UPPER  255
#define RED_US_LOWER  0

// Value Limits for red_U
#define RED_UV_UPPER  255
#define RED_UV_LOWER  178 

// Hue Limits for red_L
#define RED_LH_UPPER  10
#define RED_LH_LOWER  0

// Saturation Limits for red_L
#define RED_LS_UPPER  255
#define RED_LS_LOWER  178

// Value Limits for red_L
#define RED_LV_UPPER  255
#define RED_LV_LOWER  178

//------------ BRIGHT HSV ----------
// Hue Limits for bright
#define BRIGHT_H_UPPER  180
#define BRIGHT_H_LOWER  0

// Saturation Limits for bright
#define BRIGHT_S_UPPER  255
#define BRIGHT_S_LOWER  0

// Value Limits for bright
#define BRIGHT_V_UPPER  255
#define BRIGHT_V_LOWER  254 

void findLaser(const Mat &imageOriginal)
{
   clock_t t;

   cout << "Processing Image..." << endl;
   
   t = clock();

   Mat imageROI;
   Mat imageHSV;
   Mat imageLaser;

   // This is for the red represented at the upper end of the hue range (i.e. 170 - 180)
   Mat maskRed_U;
   Scalar redUpper_U(RED_UH_UPPER, RED_US_UPPER, RED_UV_UPPER);
   Scalar redLower_U(RED_UH_LOWER, RED_US_LOWER, RED_UV_LOWER);

   // This is for the red represented at the lower end of the hue range (i.e. 0 - 10)
   Mat maskRed_L;
   Scalar redUpper_L(RED_LH_UPPER, RED_LS_UPPER, RED_LV_UPPER);
   Scalar redLower_L(RED_LH_LOWER, RED_LS_LOWER, RED_LV_LOWER);
 
   // This is for the combined red mask (U & L or'd together)
   Mat maskRed;

   // This is for super bright regions, i.e. laser intensity is strong at close proximity
   Mat maskBright;
   Scalar brightUpper(BRIGHT_H_UPPER, BRIGHT_S_UPPER, BRIGHT_V_UPPER);
   Scalar brightLower(BRIGHT_H_LOWER, BRIGHT_S_LOWER, BRIGHT_V_LOWER);

   // This is for the laser mask (red & bright or'd together)
   Mat maskLaser;

   //// Create region of interest <---------{ !!!! THIS NEEDS TO BE UPDATED WHEN ROBOT IS BUILT !!!!
   //imageROI = imageOriginal(Rect(290, 273, 50, 50 )).clone();

   // Convert image from BGR to HSV
   //cvtColor(imageROI, imageHSV, CV_BGR2HSV);
   cvtColor(imageOriginal, imageHSV, CV_BGR2HSV);

   /************************************************************
    *  Create color masks                                      * 
    *  - red requires the combination of the U and L masks     *
    *  - laser mask requires red and bright                    *
    *                                                          *
    *  !! NOTE: The cv::compare() function does not work with  *
    *           an Array (src1) being compared to a            *
    *           Scalar (src2) as advertised, that is why I     *
    *           chose to use cv::inRange() to check for black  *
    *           values.                                        *
    ************************************************************/
   // Laser Mask
   inRange(imageHSV, redLower_U,    redUpper_U,    maskRed_U );
   inRange(imageHSV, redLower_L,    redUpper_L,    maskRed_L );
   inRange(imageHSV, brightLower,   brightUpper,   maskBright);
   bitwise_or(maskRed_U, maskRed_L, maskRed);
   bitwise_or(maskRed, maskBright, maskLaser);

   // Use masks to filter specific colors
   bitwise_and(imageOriginal, imageOriginal, imageLaser, maskLaser);

   cout << "Done...\n" << endl;

   printf("Time to process image = %f seconds\n\n",  ((float)(clock() - t)) / CLOCKS_PER_SEC);

   namedWindow( "Laser", WINDOW_AUTOSIZE );
   imshow( "Laser", imageLaser );

   namedWindow( "Laser Mask", WINDOW_AUTOSIZE );
   imshow( "Laser Mask", maskLaser );

   waitKey(0);

   //return imageRed;
   
}

#endif
