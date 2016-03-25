#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
#include "camera_rasp.h"
#include "gpio.h"
#include "color.h"

using namespace cv;
using namespace std;
using namespace raspicam;

int main(int argc, char** argv)
{

   clock_t t;

   // Create Camera Instance
   //RaspiCam Camera;
   RaspiCam_Still_Robot_Cv Camera;

   // Initialize GPIO
   wiringPiSetupGpio();
 
   // Initialize RaspiCamera
   cameraRaspInit(Camera);

   // Stabalize Camera
   usleep(RASPICAM_DELAY);

   // Copy File Name
   char *fileName = argv[1];

   Mat imageOriginal;

   Mat imageBuffer;

   // Initialize Flash Pin
   // pinMode(LED_WHITE, OUTPUT);

   for (int i = 0; i < 4; i++)
   {
      cout << "sleeping for 4 seconds" << endl;

      usleep(4000000);

      cout << "Starting..." << endl;

      t = clock();

      Color color = COLOR_UNDEFINED;

      // Turn flash ON
      digitalWrite(LED_WHITE, ON);

      // Wait for camera to stabalize
      //usleep(500000);

      //captureImage(Camera, fileName);
      captureImage(Camera, imageBuffer);

      // Turn flash off
      digitalWrite(LED_WHITE, OFF);

      color = idColor(imageBuffer);

      switch (color)
      {
         case COLOR_RED       : cout << "Container Color: RED"    << endl; break;
         case COLOR_BLUE      : cout << "Container Color: BLUE"   << endl; break;
         case COLOR_YELLOW    : cout << "Container Color: YELLOW" << endl; break;
         case COLOR_GREEN     : cout << "Container Color: GREEN"  << endl; break;
         case COLOR_UNDEFINED : cout << "Could not determine container color!" << endl; break;
      }

      cout << "Done...\n" << endl;

      printf("Time to get and process image = %f seconds\n\n",  ((float)(clock() - t)) / CLOCKS_PER_SEC);
   }

   cout << "Done with test!" << endl;
   
   t = clock();
   imwrite(fileName, imageBuffer);
   printf("Time to write image to file = %f seconds\n\n",  ((float)(clock() - t)) / CLOCKS_PER_SEC);

   t = clock();
   imageOriginal = imread(fileName, CV_LOAD_IMAGE_COLOR);
   printf("Time to read image from file = %f seconds\n\n",  ((float)(clock() - t)) / CLOCKS_PER_SEC);

   // Create file for image data
   //std::ofstream outFile( fileName, std::ios::binary );

   // Write ppm header information to image file
   //outFile << "P6\n" << Camera.getWidth() << " " << Camera.getHeight() << " 255\n";

   // Write image data to image file
   //outFile.write( (char*)imageBuffer, Camera.getImageTypeSize(RASPICAM_FORMAT_BGR));

   return 0;
}
