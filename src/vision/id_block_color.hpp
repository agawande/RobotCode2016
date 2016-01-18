#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <opencv2/core/core.hpp>

#include "camera_rasp.h"
#include "gpio.h"
#include "color.h"

using namespace cv;
using namespace std;
using namespace raspicam;

class IDBlockColor
{
public:
  IDBlockColor()
    : color(COLOR_UNDEFINED)
    {
      wiringPiSetupGpio();
      cameraRaspInit(Camera);
      usleep(RASPICAM_DELAY);

    }

  //Temporary return string, will decide what to return once sorter logic is written
  std::string
  readColor()
  {
    // Turn flash ON
    digitalWrite(LED_WHITE, ON);

    captureImage(Camera, imageBuffer);

    // Turn flash off
    digitalWrite(LED_WHITE, OFF);

    color = idColor(imageBuffer);

    switch (color)
    {
      case COLOR_RED       : return "red";
      case COLOR_BLUE      : return "blue";
      case COLOR_YELLOW    : return "yellow";
      case COLOR_GREEN     : return "green";
      case COLOR_UNDEFINED : return "error";
    }

  }

private:
  RaspiCam_Still_Robot_Cv Camera;
  Mat imageOriginal, imageBuffer;
  Color color;
};
