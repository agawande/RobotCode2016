#include <iostream>
#include <fstream>
#include <ctime>
#include <raspicam/raspicam.h>
#include "camera_1.h"

using namespace std;
using namespace raspicam;

RaspiCam Camera;


int main()
{
   cameraRaspInit(Camera);

   cout << "Height: " << Camera.getHeight() << ", Width: " << Camera.getWidth() << ", Format: " << Camera.getFormat() <<  endl;

   if ( ! Camera.open() )
   {
      cerr << "Error opening the camera" << endl;
      return -1; 
   }

   sleep(5);

   Camera.grab();

   unsigned char *data=new unsigned char[ Camera.getImageTypeSize(RASPICAM_FORMAT_BGR) ];

   Camera.retrieve(data, RASPICAM_FORMAT_IGNORE);

   std::ofstream outFile( "cameraTest_image.ppm", std::ios::binary );

   outFile << "P6\n" << Camera.getWidth() << " " << Camera.getHeight() << " 255\n";

   outFile.write( (char*)data, Camera.getImageTypeSize(RASPICAM_FORMAT_BGR));

   return 0;
}
