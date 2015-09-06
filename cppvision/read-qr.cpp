#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

class ReadQR
{
public:
  ReadQR(Mat& image)
    : qrcode(image)
  {
  }

  void displayImage()
  {
    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", qrcode);

    waitKey(0);
  }

  // Do canny edge detection, find the contours of the 3 squares on the qrcode
  void getQRContours()
  {
    Canny(qrcode, canny, 100, 200, 3);

    findContours( canny, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    int child, gchild, g2child, g3child, g4child, g5child;

    for( int i = 0; i< contours.size(); i++ )
    {
        child = hierarchy[i][3];
        gchild = hierarchy[i+1][3];
        g2child = hierarchy[i+2][3];
        g3child = hierarchy[i+3][3];
        g4child = hierarchy[i+4][3];
        g5child = hierarchy[i+5][3];
        if(g5child < -1){
            g5child = -1;
        }

       std::cout << "child: " << child << "\n";

        if(g5child == -1){
            if(child+1 == gchild && gchild+1 == g2child && g2child+1 == g3child && g3child+1 == g4child){
               std::cout << i << "\n";
               std::cout << "child: " << child << "\n";
               std::cout << "gchild: " << gchild << "\n";
               std::cout << "g2child: " << g2child << "\n";
               std::cout << "g3child: " << g3child << "\n";
               std::cout << "g4child: " << g4child << "\n";
               std::cout << "hello" << "\n";
               vector<Point> qrc = contours[i];
               qrContours.push_back(qrc);
            }
        }
    }
    //Mat drawing = qrcode;
    //for( int i = 0; i< qrContours.size(); i++ )
    //{
    //   drawContours( drawing, qrContours, i, Scalar(255,0,0), 2, 8, hierarchy, 0, Point() );
    //}

    /// Show in a window
    //namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
    //imshow( "Contours", drawing );
    //waitKey(0);

  }

  void processCentroids()
  {
    // Get Moments for all Contours and the mass centers
    vector<Moments> mu(qrContours.size());
    vector<Point2f> mc(qrContours.size());

    for( int i = 0; i < qrContours.size(); i++ ){
         mu[i] = moments( qrContours[i], false );
	 mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
    }

    int dist01, dist02, dist12;
    dist01 = cv_distance(mc[0], mc[1]);
    dist02 = cv_distance(mc[0], mc[2]);
    dist12 = cv_distance(mc[1], mc[2]);

    int a,b, topleft;
    if (dist01 > dist02 and dist01 > dist12){
      topleft = 2;
      a = 0;
      b = 1;
    }
    else if(dist02 > dist01 and dist02 > dist01){
      topleft = 1;
      a = 0;
      b = 2;
    }
    else{
      topleft = 0;
      a = 1;
      b = 2;
    }

    Mat ex = qrcode;
    centerx = (mc[a].x + mc[b].x)/2;
    centery = (mc[a].y + mc[b].y)/2;
    std::cout << centerx << "\n";
    std::cout << centery << "\n";
    //circle(ex,Point(int(centerx), int(centery)), 10, Scalar(0,0,255), -1, 8, 0 );
    //imshow("center", ex);
    //waitKey(0);

    int slope = int((mc[a].y - mc[b].y)/(mc[a].x-mc[b].x));
    int m_Per = int((centery - mc[topleft].y)/(centerx - mc[topleft].x));

    int right, bottom, top;
    if (slope > 0 && m_Per >=0 || slope < 0 && m_Per < 0){
      right = a;
      bottom = b;
    }
    else if (slope < 0 && m_Per >=0 || slope >= 0 && m_Per < 0){
      right = b;
      bottom = a;
    }
    else {  // else randomly assign, works for acute angles - i think
      right = a;
      bottom = b;
    }

    Point2f corners[3], dimen[3];
    corners[0] = farthest_point(qrContours[topleft]);
    corners[1] = farthest_point(qrContours[bottom]);
    corners[2] = farthest_point(qrContours[right]);

    dimen[0] =  Point2f( 0,0 );
    dimen[1] =  Point2f( 300,0 );
    dimen[2] =  Point2f( 0,300 );

    Mat warp_mat = getAffineTransform( corners, dimen );
    dest = qrcode;
    warpAffine(qrcode, dest, warp_mat, cv::Size(300,300));

    namedWindow( "transform", CV_WINDOW_AUTOSIZE );
    imshow( "transform", dest );
    waitKey(0);

    decodeQR();
  }

  float cv_distance(Point2f P, Point2f Q)
  {
    return sqrt(pow(abs(P.x - Q.x),2) + pow(abs(P.y - Q.y),2));
  }

  Point farthest_point(vector<Point> cnt)
  {
    int max = 0;
    int dist = 0;
    Point cmax;
    Point center = Point(int(centerx), int(centery));

    for(int i=0; i < cnt.size(); i++){
        dist = cv_distance(center,cnt[i]);
        if (dist > max){
            max = dist;
            cmax = cnt[i];
        }
    }
    return cmax;
  }

  void decodeQR(){
    imwrite("tmp.png", dest);
    system("zbarimg tmp.png");
  }

private:
  Mat& qrcode;
  Mat canny, dest;
  float centerx, centery;
  vector<vector<Point> > contours, qrContours;
  vector<Vec4i> hierarchy;
};


int main(int argc, char* argv[])
{
  Mat image;
  image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

  ReadQR readqr(image);
  readqr.getQRContours();
  readqr.processCentroids();
}
