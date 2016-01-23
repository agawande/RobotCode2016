#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <zbar.h>
#include <iostream>

using namespace cv;
using namespace std;
using namespace zbar;

/*class QRDecode
{
public:
  QRDecode()
    {
    }

private:
  Mat frame, grey;
}*/

int main(int argc, char* argv[])
{
  clock_t t1,t2;
  t1=clock();
  Mat frame, grey;

  frame = imread(argv[1]);

  cvtColor(frame,grey,CV_BGR2GRAY);

  int width = frame.cols;
  int height = frame.rows;
  uchar *raw = (uchar *)grey.data;

  Image image(width, height, "Y800", raw, width * height);

  ImageScanner scanner;
  scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

  int n = scanner.scan(image);

  for(Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol)
  {
       vector<Point> vp;

       cout << "decoded " << symbol->get_type_name()  << " symbol \"" << symbol->get_data() << '"' <<" "<< endl;
       int n = symbol->get_location_size();

       for(int i=0;i<n;i++){
         vp.push_back(Point(symbol->get_location_x(i),symbol->get_location_y(i)));
       }
  }

  t2=clock();
  float diff ((float)t2-(float)t1);
  float seconds = diff/CLOCKS_PER_SEC;
  cout<<seconds<<endl;
}
