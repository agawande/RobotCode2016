#ifndef COURSE_READER_HPP
#define COURSE_READER_HPP

#include <iostream>
#include "complex.hpp"
#include <fstream>

using namespace std;

class CourseReader {

  public:
    //Constructors
    CourseReader() { numLines = 0; currentLine = 0; }

    //Destructor
    ~CourseReader();

    //public functions
    Complex* next();
    Complex* atIndex( int );

    void readFileIntoArray();
    void setFileName( const string & );

  private:
    //private objecys
    Complex* processLine( string );
    ifstream fileStream;
    string nextLine, fileName;
    Complex **complexArray;
    int numLines, currentLine;
};

#endif
