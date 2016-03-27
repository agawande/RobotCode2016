#include <unistd.h>
#include <opencv2/core/core.hpp>

#include "camera.hpp"

using namespace std;
using namespace cv;
using namespace raspicam;

Camera::Camera() : RaspiCam_Still_Robot_Cv()
{
   cameraInit();

   // Stabalize Camera
   usleep(RASPICAM_DELAY);
 
} //Camera

void Camera::cameraInit()
{
   /************************************************************************
   // Set default parameters
   //setWidth(RASPICAM_WIDTH);
   //setHeight(RASPICAM_HEIGHT);
   RaspiCam_Still_Robot_Cv::setCaptureSize(RASPICAM_WIDTH, RASPICAM_HEIGHT);
   RaspiCam_Still_Robot_Cv::setBrightness(RASPICAM_BRIGHTNESS);
   //RaspiCam_Still_Robot_Cv::setISO(RASPICAM_ISO);
   RaspiCam_Still_Robot_Cv::setVerticalFlip(YES);
   //RaspiCam_Still_Robot_Cv::setHorizontalFlip(YES);
   RaspiCam_Still_Robot_Cv::setExposure(RASPICAM_EXPOSURE_AUTO);
   RaspiCam_Still_Robot_Cv::setAWB(RASPICAM_AWB_AUTO);
   //RaspiCam_Still_Robot_Cv::setAWB(RASPICAM_AWB_INCANDESCENT);
   RaspiCam_Still_Robot_Cv::setImageEffect(RASPICAM_IMAGE_EFFECT_BLUR);
   RaspiCam_Still_Robot_Cv::setMetering(RASPICAM_METERING_SPOT);
   //RaspiCam_Still_Robot_Cv::setMetering(RASPICAM_METERING_MATRIX);
   ************************************************************************/

   // Set default parameters
   //setWidth(RASPICAM_WIDTH);
   //setHeight(RASPICAM_HEIGHT);
   setCaptureSize(RASPICAM_WIDTH, RASPICAM_HEIGHT);
   setBrightness(RASPICAM_BRIGHTNESS);
   //setISO(RASPICAM_ISO);
   setVerticalFlip(YES);
   //setHorizontalFlip(YES);
   setExposure(RASPICAM_EXPOSURE_AUTO);
   setAWB(RASPICAM_AWB_AUTO);
   //setAWB(RASPICAM_AWB_INCANDESCENT);
   setImageEffect(RASPICAM_IMAGE_EFFECT_BLUR);
   setMetering(RASPICAM_METERING_SPOT);
   //setMetering(RASPICAM_METERING_MATRIX);

   if ( ! open() )
   {
      cerr << "Error opening the camera" << endl;
   }

   
} //cameraInit

Mat Camera::captureImage()
{
   // Acquire image
   grab();

   // Move image from buffer to memory
   retrieve(imageBuffer);

   return imageBuffer;

} //captureImage
