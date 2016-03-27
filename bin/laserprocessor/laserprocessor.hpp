#ifndef LASERPROCESSOR_HPP
#define LASERPROCESSOR_HPP

#include <opencv2/core/core.hpp>

using namespace cv;


class LaserProcessor : public Camera
{
   public:
      LaserProcessor();
      unsigned int getDistance();
      unsigned int getGapCenter();
      double getWallAngle();
      double getBlockAngle();

   private:
      unsigned int scanColumnMiddle;
      unsigned int scanColumnLeft;
      unsigned int scanColumnRight;

      //Private Method
      unsigned int laserDistanceMeasurement(const Mat & laserMask);
      Mat findLaser(Mat & image); // Creates laserMask
      unsigned int laserDistanceCheck(unsigned int laserCenter);
      double laserPerpendicularityCheck(unsigned int leftColumn, unsigned int rightColumn, unsigned int width);
      unsigned int findLaserHeight(Mat & laserMask, unsigned int searchColumn);
}

#endif
