#include "course_reader.h"
#include "complex.h"
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

CourseReader::CourseReader( string &fileN )
 : fileStream(fileN.c_str())
{
  setFileName(fileN);
}

CourseReader::~CourseReader()
{
  fileStream.close();
}

Complex
CourseReader::next() {
  getline(fileStream, nextLine);

  //lineNum++;
  return processLine(nextLine);
}

Complex
CourseReader::atIndex( int index ) {
  int i = 0;
  ifstream infile(fileName.c_str());

  string str;
  while (std::getline(infile, str))
  {
    if(i == index){
        Complex a = processLine(str);

        infile.close();
        return a;
    }
    i++;
  }
  //close stream
  infile.close();
}

Complex
CourseReader::processLine(string line) {
  string myArray[4];

  stringstream streamStr(line);

  int i;
  vector<int> vect;

  while (streamStr >> i)
  {
    vect.push_back(i);

    if (streamStr.peek() == ',') {
      streamStr.ignore();
    }
  }

  Complex a(vect.at(0), vect.at(1), vect.at(2), vect.at(3));
  return a;
}


void
CourseReader::setFileName(string &fileN) {
  struct stat buffer;
  if((stat (fileN.c_str(), &buffer) == 0)) {
    fileName = fileN;
  } else {
    cout << fileName << " does not exist" << endl;
    exit(-1);
  }
}
