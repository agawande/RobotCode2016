#ifndef LASERPROCESSOR_HPP
#define LASERPROCESSOR_HPP

#include <opencv2/core/core.hpp>

#include "camera.hpp"
#include "laserprocessor.h"
#include "gpio.hpp"

using namespace cv;

class LaserProcessor : public Camera, public Gpio
{
   public:
      LaserProcessor();
      unsigned int getBlockDistance(const Mat &imageOriginal);
      unsigned int getBlockDistance();
      unsigned int getWallDistance(const Mat &imageOriginal);
      unsigned int getWallDistance();
      unsigned int getGapCenter();
      double getWallAngle();
      double getBlockAngle();

   private:
      unsigned int scanColumnMiddle;
      unsigned int scanColumnLeft;
      unsigned int scanColumnRight;

      //Private Method
      Mat findLaser(const Mat &imageOriginal); // Creates laserMask
      double calculateAngle(const Mat &imageOriginal, unsigned int leftColumn, unsigned int rightColumn, unsigned int width);
      unsigned int laserDistanceCheck(const Mat &laserMask, unsigned int searchColumn);
};

#endif
