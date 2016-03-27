#ifndef COLORPROCESSOR_HPP
#define COLORPROCESSOR_HPP

#include <opencv2/core/core.hpp>

#include "camera.hpp"
#include "colorprocessor.h"

using namespace cv;

class ColorProcessor : public Camera
{
   public: 
      
      ColorProcessor();

      Color idBlock(const Mat &imageOriginal);
      Color idBlock();
      Color idHopper(const Mat &imageOriginal);
      Color idHopper();

   private:
      Color idColor(const Mat &imageOriginal);

}; //ColorProcessor

#endif
