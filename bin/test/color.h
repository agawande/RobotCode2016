#ifndef COLOR_H
#define COLOR_H

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

Color idColor(const Mat &imageOriginal)
{
   Mat imageROI;
   Mat imageHSV;
   Mat imageRed;
   Mat imageBlue;
   Mat imageYellow;
   Mat imageGreen;

   Color color = COLOR_UNDEFINED;

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

   Mat maskBlue;
   Scalar blueUpper(BLUE_H_UPPER, BLUE_S_UPPER, BLUE_V_UPPER);
   Scalar blueLower(BLUE_H_LOWER, BLUE_S_LOWER, BLUE_V_LOWER);

   Mat maskYellow;
   Scalar yellowUpper(YELLOW_H_UPPER, YELLOW_S_UPPER, YELLOW_V_UPPER);
   Scalar yellowLower(YELLOW_H_LOWER, YELLOW_S_LOWER, YELLOW_V_LOWER);

   Mat maskGreen;
   Scalar greenUpper(GREEN_H_UPPER, GREEN_S_UPPER, GREEN_V_UPPER);
   Scalar greenLower(GREEN_H_LOWER, GREEN_S_LOWER, GREEN_V_LOWER);

   // At this time black will be or'd with green values, could change based on lighting
   Mat maskBlack;
   Scalar black(0,0,0);

   // Create region of interest <---------{ !!!! THIS NEEDS TO BE UPDATED WHEN ROBOT IS BUILT !!!!
   //imageROI = imageOriginal(Rect(290, 273, 50, 50 )).clone();
   imageROI = imageOriginal(Rect(290, 435, 50, 35 )).clone();

   // Convert image from BGR to HSV
   cvtColor(imageROI, imageHSV, CV_BGR2HSV);

   // For average color in ROI, Need to add one for the undefined value
   unsigned int colorMatchAverages[NUM_COLORS + 1];

   /************************************************************
    *  Create color masks                                      * 
    *  - red requires the combination of the U and L masks     *
    *  - We are going to assume that black is green            *
    *                                                          *
    *  !! NOTE: The cv::compare() function does not work with  *
    *           an Array (src1) being compared to a            *
    *           Scalar (src2) as advertised, that is why I     *
    *           chose to use cv::inRange() to check for black  *
    *           values.                                        *
    ************************************************************/
   // RED Mask
   inRange(imageHSV, redLower_U,    redUpper_U,    maskRed_U);
   inRange(imageHSV, redLower_L,    redUpper_L,    maskRed_L);
   bitwise_or(maskRed_U, maskRed_L, maskRed);

   // BLUE Mask
   inRange(imageHSV, blueLower,   blueUpper,   maskBlue);
 
   // YELLOW Mask
   inRange(imageHSV, yellowLower, yellowUpper, maskYellow);

   // GREEN Mask
   inRange(imageHSV, greenLower,  greenUpper,  maskGreen);
   inRange(imageHSV, black, black, maskBlack);
   bitwise_or(maskGreen, maskBlack, maskGreen);

   // Determine the average value for each color mask (value will be between 0 & 255, inclusive)
   colorMatchAverages[COLOR_RED]       = mean(maskRed).val[0];
   colorMatchAverages[COLOR_BLUE]      = mean(maskBlue).val[0];
   colorMatchAverages[COLOR_YELLOW]    = mean(maskYellow).val[0];
   colorMatchAverages[COLOR_GREEN]     = mean(maskGreen).val[0];
   colorMatchAverages[COLOR_UNDEFINED] = 0;


  /**********************************************************************
   * Identify color based on strongest match above the given threshold  *
   *                                                                    *
   *   !! NOTE: We are not iterating over the COLOR_UNDEFINED value.    *
   *            That was only used to enforce a default value of 0      *
   **********************************************************************/
   for (int i = 0; i < NUM_COLORS; i++)
   {
      // The else condition is implied (i.e. else {color = color})
      if ((colorMatchAverages[i] > COLOR_THRESHOLD) && colorMatchAverages[i] > colorMatchAverages[color])
      {
         color = static_cast<Color>(i);
      } 
   }

   /**********************************************************

   cout << "Red Match = " << colorMatchAverages[COLOR_RED] << endl;
   cout << "Blue Match = " << colorMatchAverages[COLOR_BLUE] << endl;
   cout << "Yellow Match = " << colorMatchAverages[COLOR_YELLOW] << endl;
   cout << "Green Match = " << colorMatchAverages[COLOR_GREEN] << endl;

   // Use masks to filter specific colors
   bitwise_and(imageROI, imageROI, imageRed,    maskRed);
   bitwise_and(imageROI, imageROI, imageBlue,   maskBlue);
   bitwise_and(imageROI, imageROI, imageYellow, maskYellow);
   bitwise_and(imageROI, imageROI, imageGreen,  maskGreen);

   namedWindow( "Red", WINDOW_AUTOSIZE );
   imshow( "Red", imageRed );

   namedWindow( "Blue", WINDOW_AUTOSIZE );
   imshow( "Blue", imageBlue );

   namedWindow( "Yellow", WINDOW_AUTOSIZE );
   imshow( "Yellow", imageYellow );

   namedWindow( "Green", WINDOW_AUTOSIZE );
   imshow( "Green", imageGreen );
    
   waitKey(0);
   ***********************************************************/
   
   return color;
}

#endif
