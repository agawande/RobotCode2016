#include <iostream>
//#include <time.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "laserProcessor.hpp"

using namespace cv;
using namespace std;

Mat findLaser(Mat &imageOriginal)
{
   Mat imageROI;
   Mat imageHSV;
   Mat imageLaser;

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

   // Create region of interest <---------{ !!!! THIS NEEDS TO BE UPDATED WHEN ROBOT IS BUILT !!!!
   imageROI = imageOriginal(Rect(ROI_X, ROI_Y, ROI_WIDTH, ROI_HEIGHT )).clone();

   // Convert image from BGR to HSV
   cvtColor(imageROI, imageHSV, CV_BGR2HSV);
   imageHSV = GaussianBlur(imageHSV,(5,5),0);

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
   inRange(imageHSV, laserRedMin_U,    laserRedMax_U,    maskLaserRed_U );
   inRange(imageHSV, laserRedMin_L,    laserRedMax_L,    maskLaserRed_L );
   inRange(imageHSV, laserBrightMin,   laserBrightMax,   maskLaserBright);
   bitwise_or(maskLaserRed_U, maskLaserRed_L, maskLaserRed);
   bitwise_or(maskLaserRed, maskLaserBright, maskLaser);


   // Use masks to filter specific colors
   //bitwise_and(imageOriginal, imageOriginal, maskLaser, imageLaser);
   bitwise_and(imageROI, imageROI, imageLaser, maskLaser);


   //clock_t t;
   //cout << "Processing Image..." << endl;
   //t = clock();
   //cout << "Done...\n" << endl;
   //printf("Time to process image = %f seconds\n\n",  ((float)(clock() - t)) / CLOCKS_PER_SEC);
   //namedWindow( "Laser", WINDOW_AUTOSIZE );
   //imshow( "Laser", imageLaser );
   //namedWindow( "Laser Mask", WINDOW_AUTOSIZE );
   //imshow( "Laser Mask", maskLaser );
   //waitKey(0);

   cout << "mask built, bye bye" << endl;

   return imageLaser;
   
}

unsigned int laserProximityCheck(Mat & laserMask, unsigned int & targetHeight)
{
   unsigned int maxTargetHeight = 0; //Upper boundary of target band
   unsigned int minTargetHeight = 0; //Lower boundary of target band
 
   unsigned int laserUpperBounds = 0; //Laser's measured upper boundary
   unsigned int laserLowerBounds = 0; //Laser's measured lower boundary

   unsigned int upperRange = targetHeight + PROXIMITY_RANGE; //Upper Close Range
   unsigned int lowerRange = targetHeight - PROXIMITY_RANGE; //Lower Close Range

   unsigned int laserCenter = 0; //Laser's centerpoint

   uchar previousValue = BLACK;

   // We are going to scan from the top of this column to find the first 1 and then the last 1 in the column
   unsigned int scanColumn = ROI_X + (ROI_WIDTH / 2); //Roughly the middle column of the ROI

   // Image Properties
   int nRows        = laserMask.rows;
   int nColumns     = laserMask.cols;

   // This sets up the target band we want to move into
   if ( PROXIMITY_ALLOWANCE > 0 && PROXIMITY_ALLOWANCE <= 1 ){
      maxTargetHeight = targetHeight * (1 + PROXIMITY_ALLOWANCE); 
      minTargetHeight = targetHeight / (1 + PROXIMITY_ALLOWANCE); 
   }
   else{
      //Default values will give us a little spread to ensure we can move into the correct location
      maxTargetHeight = targetHeight + PROXIMITY_RANGE;
      minTargetHeight = targetHeight - PROXIMITY_RANGE;
   }

   // Pointer to row i in the image matrix
   uchar *p;

   // If the image is stored continuously adjust the num of columns & rows accordingly
   if ( laserMask.isContinuous() ){
 
      nColumns *= nRows;
      nRows     = 1;

      p = laserMask.ptr<uchar>(0); //Assigning the first row because matrix is a continuous array

      // Must increment counter by the number of rows in the image for each iteration
      // this in effect moves the pointer down one row within the same column
      for ( int i = 0, row = 0; i < nColumns; i *= laserMask.cols, row++ ){

         // Look for upper boundary of laser in image
         if ( (previousValue == BLACK) && (p[i] == WHITE) ){
            laserUpperBounds = row;
         }

         // Look for lower boundary of laser in image, exiting on first lower boundary detected
         if ( (previousValue == WHITE) && (p[i] == BLACK) ){  
            laserLowerBounds = row;
         }
      
         
      }
   }
   else {

      // Scan down from the top of the SAME column in each row
      for( int i = 0; i < nRows ; i++){

         p = laserMask.ptr<uchar>(i); //Traverses each row

         // Look for upper boundary of laser in image
         if ( (previousValue == BLACK) && (p[scanColumn] == WHITE) ){
            laserUpperBounds = i;
         }

         // Look for lower boundary of laser in image, exiting on first lower boundary detected
         if ( (previousValue == WHITE) && (p[scanColumn] == BLACK) ){  
            laserLowerBounds = i;
         }
      
      }  

   }
 
   // Estimating the centerpoint of the laser based on the upper and lower bounds
   laserCenter = (laserUpperBounds - laserLowerBounds) / 2;
   
   // Stop if laser center is within target range
   if ( (laserCenter < maxTargetHeight) && (laserCenter > minTargetHeight) ){
      return STOP;
   }
   else { //Using this to ENSURE that we return out of this function with a valid command

      if (laserCenter > maxTargetHeight){
         if (laserCenter > upperRange){
            return MOVE_BACKWARD_FAST;
         }
         else{
            return MOVE_BACKWARD_SLOW;
         }
      } //Close move backward block 
   
      if (laserCenter < minTargetHeight){
         if (laserCenter < lowerRange){
            return MOVE_FORWARD_FAST;
         }
         else{
            return MOVE_FORWARD_SLOW;
         }
      } //Close move forward block

   } 
 
} //Close laserProximityCheck()
