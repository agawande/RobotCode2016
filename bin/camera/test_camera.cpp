#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include "camera.hpp"

//#include <unistd.h>
//#include <time.h>

using namespace cv;
using namespace std;
using namespace raspicam;

int main(int argc, char** argv)
{
   clock_t t;

   // Create Camera Instance
   Camera camera;

   // Copy File Name
   char *fileName = argv[1];

   cout << "Capturing Image..." << endl;

   Mat imageBuffer;

   t = clock();

   imageBuffer = camera.captureImage();

   cout << "Done...\n" << endl;

   printf("Time to capture image = %f seconds\n\n",  ((float)(clock() - t)) / CLOCKS_PER_SEC);


   cout << "Writing Image to File..." << endl;

   t = clock();

   imwrite(fileName, imageBuffer);

   cout << "Done...\n" << endl;

   printf("Time to write image = %f seconds\n\n",  ((float)(clock() - t)) / CLOCKS_PER_SEC);

   return 0;
}
