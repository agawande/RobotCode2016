#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <iostream>
#include <ctime>
#include <opencv2/core/core.hpp>
#include <raspicam/raspicam_still_robot_cv.h>

#define RASPICAM_BRIGHTNESS 50
#define RASPICAM_ISO        800
#define RASPICAM_WIDTH      640
#define RASPICAM_HEIGHT     480
#define RASPICAM_DELAY      650000 //microseconds
#define YES                 1

using namespace cv;
using namespace raspicam;

class Camera : RaspiCam_Still_Robot_Cv
{
   public:
      Camera();
      Mat captureImage();

   private:
      Mat imageBuffer;
      void cameraInit();

}; //Camera

#endif
