#include <iostream>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "laserprocessor.hpp"
#include "laserprocessor.h"

using namespace cv; 
using namespace std;

LaserProcessor::LaserProcessor() : Camera() , Gpio()
{
   scanColumnMiddle = (ROI_WIDTH / 2); //Roughly the middle column of the ROI
   scanColumnLeft   = 0; //Left most Column
   scanColumnRight  = ROI_WIDTH; //Right most column
}

unsigned int LaserProcessor::getBlockDistance( const Mat &imageOriginal )
{
   Mat laserMask = findLaser( imageOriginal(Rect(ROI_X, ROI_Y, ROI_WIDTH, ROI_HEIGHT )).clone() );

   return laserDistanceCheck( laserMask, scanColumnMiddle );
    
} //getBlockDistance

unsigned int LaserProcessor::getBlockDistance() // Takes picture if no argument specified
{
   laserOn();

   Mat laserMask = findLaser( captureImage()(Rect(ROI_X, ROI_Y, ROI_WIDTH, ROI_HEIGHT )).clone() );

   laserOff();

   return laserDistanceCheck( laserMask, scanColumnMiddle );

} //getBlockDistance

Mat LaserProcessor::findLaser(const Mat &imageOriginal)
{
   Mat imageROI;
   Mat imageHSV;
   Mat filteredImage;
   Mat imageLaser;

   // Used to filter for horizontal components
   uchar filterKernalValues[FILTER_KERNAL_SIZE][FILTER_KERNAL_SIZE] = 
      { 
         {0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0},
         {1, 1, 1, 1, 1, 1, 1, 1, 1},
         {0, 0, 0, 0, 0, 0, 0, 0, 0}, 
         {0, 0, 0, 0, 0, 0, 0, 0, 0}, 
         {0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0}
      };

   Mat filterKernal(Size(FILTER_KERNAL_SIZE, FILTER_KERNAL_SIZE), CV_8U, &filterKernalValues);

   // This is for the red represented at the upper end of the hue range (i.e. 170 - 180)
   Mat maskLaserRed_U;
   Scalar laserRedMax_U(LASER_RED_UH_MAX, LASER_RED_US_MAX, LASER_RED_UV_MAX);
   Scalar laserRedMin_U(LASER_RED_UH_MIN, LASER_RED_US_MIN, LASER_RED_UV_MIN);

   // This is for the red represented at the lower end of the hue range (i.e. 0 - 10)
   Mat maskLaserRed_L;
   Scalar laserRedMax_L(LASER_RED_LH_MAX, LASER_RED_LS_MAX, LASER_RED_LV_MAX);
   Scalar laserRedMin_L(LASER_RED_LH_MIN, LASER_RED_LS_MIN, LASER_RED_LV_MIN);
 
   // This is for the combined red mask (U & L or'd together)
   Mat maskLaserRed;

   // This is for super bright regions, i.e. laser intensity is strong at close proximity
   Mat maskLaserBright;
   Scalar laserBrightMax(LASER_BRIGHT_H_MAX, LASER_BRIGHT_S_MAX, LASER_BRIGHT_V_MAX);
   Scalar laserBrightMin(LASER_BRIGHT_H_MIN, LASER_BRIGHT_S_MIN, LASER_BRIGHT_V_MIN);

   // This is for the laser mask (red & bright or'd together)
   Mat maskLaser;

   // Convert image from BGR to HSV
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

   // Create Individual Masks
   inRange(imageHSV, laserRedMin_U,    laserRedMax_U,    maskLaserRed_U );
   inRange(imageHSV, laserRedMin_L,    laserRedMax_L,    maskLaserRed_L );
   inRange(imageHSV, laserBrightMin,   laserBrightMax,   maskLaserBright);

   // Combine Masks
   bitwise_or(maskLaserRed_U, maskLaserRed_L, maskLaserRed);
   bitwise_or(maskLaserRed, maskLaserBright, maskLaser);

   // Apply Combined Mask
   bitwise_and(imageOriginal, imageOriginal, imageLaser, maskLaser);

   // Filter mask to clean it up, should result in a thicker white line against black background
   blur(maskLaser, maskLaser, Size(FILTER_KERNAL_SIZE, FILTER_KERNAL_SIZE));
   threshold(maskLaser, maskLaser, FILTER_THRESHOLD, WHITE, THRESH_BINARY);
   filter2D(maskLaser, maskLaser, -1, filterKernal, Point(-1,-1), 0, BORDER_CONSTANT);

   return maskLaser;
   
}


unsigned int LaserProcessor::laserDistanceCheck(const Mat &laserMask, unsigned int searchColumn)
{

   // Markers for transition checks
   unsigned int laserUpperBounds = 0; //Laser's measured upper boundary
   unsigned int laserLowerBounds = 0; //Laser's measured lower boundary

   // Measured laser's average center point
   unsigned int laserCenter = 0; 

   // Pixel value
   unsigned int pixelValue = 0;

   // Set previous value - we are looking for transitions between black & white
   uchar previousValue = BLACK;

   for(int i = 0; i < laserMask.rows; i++){
      pixelValue = (unsigned int)laserMask.at<uchar>(i, searchColumn);

      // Look for upper boundary of laser in image
      if ( (previousValue == BLACK) && ( pixelValue == WHITE) ){
         previousValue = WHITE; //Set Previous Value to white
         laserUpperBounds = i; //Mark row which white value was found
      }

      // Look for lower boundary of laser in image, exiting on first lower boundary detected
      if ( (previousValue == WHITE) && (pixelValue == BLACK) ){  
         laserLowerBounds = i; //Mark row which black value was found
         break; //Exit the loop
      }

   }

   // Estimating the centerpoint of the laser based on the upper and lower bounds
   laserCenter = laserUpperBounds + ((laserLowerBounds - laserUpperBounds) / 2);

   return distanceLookup[laserCenter - 1];

} // close laserDistanceCheck


double LaserProcessor::calculateAngle(const Mat &imageOriginal, unsigned int leftColumnHeight, unsigned int rightColumnHeight, unsigned int width)
{

   int leftHeight  = laserDistanceCheck( imageOriginal, leftColumnHeight );
   int rightHeight = laserDistanceCheck( imageOriginal, rightColumnHeight );

   int minDistance = min(leftHeight, rightHeight);

   cout << "Change in Height = " << rightHeight - leftHeight << endl;

   //cout << "Converted Width = " << convertedWidth << endl;
   cout << "Left Height = " << leftHeight << ", Right Hight = " << rightHeight << ", Width = " << width << endl;

   // Returning angle (have to multiply by -1 to fix the fact that height is measured from the top down
   //return atan( ((double)rightHeight - (double)leftHeight) / (double)convertedWidth ) * 180 / PI * -1;
   return atan( ((double)rightHeight - (double)leftHeight) / 1.0 ) ;

}// Close laserPerpendicularityCheck
