#ifndef CAMERA_RASP_H
#define CAMERA_RASP_H

#include <iostream>
//#include <fstream>
#include <ctime>
#include <opencv2/core/core.hpp>
//#include <raspicam/raspicam.h>
#include <raspicam/raspicam_still_robot_cv.h>

#define RASPICAM_BRIGHTNESS 50
#define RASPICAM_ISO        800
#define RASPICAM_WIDTH      640
#define RASPICAM_HEIGHT     480
#define RASPICAM_DELAY      650000 //microseconds
#define YES                 1

using namespace std;
using namespace cv;
using namespace raspicam;

//void cameraRaspInit(RaspiCam &Camera)
void cameraRaspInit(RaspiCam_Still_Robot_Cv &Camera)
{

   // Set default parameters
   //Camera.setWidth(RASPICAM_WIDTH);
   //Camera.setHeight(RASPICAM_HEIGHT);
   Camera.setCaptureSize(RASPICAM_WIDTH, RASPICAM_HEIGHT);
   Camera.setBrightness(RASPICAM_BRIGHTNESS);
   //Camera.setISO(RASPICAM_ISO);
   //Camera.setVerticalFlip(YES);
   //Camera.setHorizontalFlip(YES);
   Camera.setExposure(RASPICAM_EXPOSURE_AUTO);
   Camera.setAWB(RASPICAM_AWB_AUTO);
   //Camera.setAWB(RASPICAM_AWB_INCANDESCENT);
   Camera.setImageEffect(RASPICAM_IMAGE_EFFECT_BLUR);
   Camera.setMetering(RASPICAM_METERING_SPOT);
   //Camera.setMetering(RASPICAM_METERING_MATRIX);

   if ( ! Camera.open() )
   {
      cerr << "Error opening the camera" << endl;
   }

   
}

//void captureImage(RaspiCam &Camera, char* fileName)
void captureImage( RaspiCam_Still_Robot_Cv &Camera, Mat &imageBuffer)
{
   // Acquire image
   Camera.grab();

   // Move image from buffer to memory
   Camera.retrieve(imageBuffer);

   // Reserve memory for buffered data
   ////////////unsigned char *data=new unsigned char[ Camera.getImageTypeSize(RASPICAM_FORMAT_BGR) ];

   // Move image from buffer to memory
   ////////////Camera.retrieve(data, RASPICAM_FORMAT_IGNORE);

   // Create file for image data
   //std::ofstream outFile( fileName, std::ios::binary );

   // Write ppm header information to image file
   //outFile << "P6\n" << Camera.getWidth() << " " << Camera.getHeight() << " 255\n";

   // Write image data to image file
   //outFile.write( (char*)data, Camera.getImageTypeSize(RASPICAM_FORMAT_BGR));

}
 

#endif
