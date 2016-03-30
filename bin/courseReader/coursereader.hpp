#ifndef COURSE_READER_HPP
#define COURSE_READER_HPP

#include <iostream>
#include "coordinate.hpp"
#include <fstream>

using namespace std;

class CourseReader {

  public:
    //Constructors
    CourseReader() { numLines = 0; currentLine = 0; }

    //Destructor
    ~CourseReader();

    //public functions
    Coordinate* next();
    Coordinate* atIndex( int );

    void readFileIntoArray();
    void setFileName( const string & );

  private:
    //private objecys
    Coordinate* processLine( string );
    ifstream fileStream;
    string nextLine, fileName;
    Coordinate **coordinateArray;
    int numLines, currentLine;
};

#endif
