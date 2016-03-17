#include <iostream>
#include "complex.h"
#include <fstream>

using namespace std;

class CourseReader {
  public:
    CourseReader( string & );
    ~CourseReader();

    Complex next();
    Complex atIndex( int );

  private:
    void setFileName( string & );
    Complex processLine( string );
    ifstream fileStream;
    string nextLine, fileName;
};
