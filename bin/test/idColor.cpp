#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <opencv2/core/core.hpp>
#include "camera_rasp.h"
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

   // Initialize RaspiCamera
   cameraRaspInit(Camera);

   // Stabalize Camera
   usleep(RASPICAM_DELAY);

   // Copy File Name
   char *fileName = argv[1];

   Mat imageOriginal;

   Mat imageBuffer;

   for (int i = 0; i < 4; i++)
   {
      cout << "sleeping for 4 seconds" << endl;

      usleep(4000000);

      cout << "Starting..." << endl;

      t = clock();

      Color color = COLOR_UNDEFINED;

      // Wait for camera to stabalize
      //usleep(500000);

      //captureImage(Camera, fileName);
      captureImage(Camera, imageBuffer);

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
   
   return 0;
}
