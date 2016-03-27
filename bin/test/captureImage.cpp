#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include "camera_rasp.h"

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

   Mat imageBuffer;

   cout << "Capturing Image..." << endl;

   t = clock();

   captureImage(Camera, imageBuffer);

   cout << "Done...\n" << endl;

   printf("Time to capture image = %f seconds\n\n",  ((float)(clock() - t)) / CLOCKS_PER_SEC);


   cout << "Writing Image to File..." << endl;

   t = clock();

   imwrite(fileName, imageBuffer);

   cout << "Done...\n" << endl;

   printf("Time to write image = %f seconds\n\n",  ((float)(clock() - t)) / CLOCKS_PER_SEC);

   return 0;
}
