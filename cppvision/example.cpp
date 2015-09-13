#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

int main ( int argc, char **argv ){

VideoCapture capture(0);

if(!capture.isOpened()){  // check if we succeeded
   return -1;
}

Mat image;

capture >> image;

namedWindow("Display Image", WINDOW_AUTOSIZE );
imshow("Display Image", image);

waitKey(0);

}
