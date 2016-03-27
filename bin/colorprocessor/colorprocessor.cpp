#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "colorprocessor.h"
#include "colorprocessor.hpp"

using namespace cv;

ColorProcessor::ColorProcessor() : Camera(){}


Color ColorProcessor::idBlock(const Mat &imageOriginal)
{
   //Specific ROI for block is specified below, do not change unless absolutely necessary
   return idColor( imageOriginal(Rect(290, 435, 50, 35 )).clone() );
} //idBlock


Color ColorProcessor::idBlock() // <-----{ takes picture if no argument specified
{
   //Specific ROI for block is specified below, do not change unless absolutely necessary
   return idColor( captureImage()(Rect(290, 435, 50, 35 )).clone() );
} //idBlock() 


Color ColorProcessor::idHopper(const Mat &imageOriginal)
{
   //Specific ROI for hopper is specified below, do not change unless absolutely necessary
   return idColor ( imageOriginal(Rect(290, 435, 50, 35 )).clone() );
} //idHopper


Color ColorProcessor::idHopper() // <-----< takes picture if no argument specified
{
   //Specific ROI for hopper is specified below, do not change unless absolutely necessary
   return idColor( captureImage()(Rect(290, 435, 50, 35 )).clone() );
} //idHopper


Color ColorProcessor::idColor(const Mat &imageROI)
{
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

   /**********************************************************/

   return color;
} //idColor

