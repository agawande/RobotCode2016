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
   Mat imageOriginal;
   Mat imageBuffer;

   clock_t t;

   // Copy File Name
   char *fileName = argv[1];

   imageOriginal = imread(fileName, CV_LOAD_IMAGE_COLOR);

   t = clock();

   Color color = COLOR_UNDEFINED;

   color = idColor(imageOriginal);

   switch (color)
   {
      case COLOR_RED       : cout << "Container Color: RED"    << endl; break;
      case COLOR_BLUE      : cout << "Container Color: BLUE"   << endl; break;
      case COLOR_YELLOW    : cout << "Container Color: YELLOW" << endl; break;
      case COLOR_GREEN     : cout << "Container Color: GREEN"  << endl; break;
      case COLOR_UNDEFINED : cout << "Could not determine container color!" << endl; break;
   }

   printf("Time to read image from file = %f seconds\n\n",  ((float)(clock() - t)) / CLOCKS_PER_SEC);

   return 0;
}
