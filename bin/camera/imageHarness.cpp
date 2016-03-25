#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "laserProcessor.hpp"
using namespace cv;
using namespace std;

int main(int argc, char** argv)
{

   clock_t t;

   // Copy File Name
   char *imageFile = argv[1];

   Mat imageOriginal;
   Mat imageTargetRange;

   Mat laserMask;

   unsigned int targetHeight = 50;

   cout << "Loading Image: " << imageFile << endl;
   t = clock();
   imageOriginal = imread(imageFile, CV_LOAD_IMAGE_COLOR);
   printf("Time to Load  image = %f seconds\n\n",  ((float)(clock() - t)) / CLOCKS_PER_SEC);

   t = clock();
   laserMask = findLaser(imageOriginal);

   cout << "Mask Size " << laserMask.size() << endl;
   cout << "Mask Channels " << laserMask.channels() << endl;

   // We are going to scan from the top of this column to find the first 1 and then the last 1 in the column
   unsigned int scanColumn = (ROI_WIDTH / 2); //Roughly the middle column of the ROI

   unsigned int laserHeight = findLaserHeight(laserMask, scanColumn);
   cout << "Laser Height = " << laserHeight << endl;

   unsigned int distance = laserDistanceCheck(laserHeight);
   cout << "Measured distance to center = " << distance << endl;

   unsigned int leftHeight = findLaserHeight(laserMask, 100);
   unsigned int rightHeight= findLaserHeight(laserMask, 175);
   double slope = laserPerpendicularityCheck(leftHeight, rightHeight, ROI_WIDTH);

   cout << "Angle = " << slope << " radians" << endl;

   printf("Time to process Image = %f seconds\n\n",  ((float)(clock() - t)) / CLOCKS_PER_SEC);

   cout << "Displaying Original Image" << endl;
   namedWindow( "Original Image", WINDOW_AUTOSIZE );
   imshow( "Original Image", imageOriginal );

   cout << "Displaying Laser Mask" << endl;
   namedWindow( "Laser Mask", WINDOW_AUTOSIZE );
   imshow( "Laser Mask", laserMask );
   
   waitKey(0);

   imwrite("angleLaserMask.png", laserMask);

   return 0;
}
