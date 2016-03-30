#include "coursereader.hpp"
#include "coordinate.hpp"
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

CourseReader::~CourseReader()
{
  delete coordinateArray;
}

void
CourseReader::readFileIntoArray()
{
  while (getline(fileStream, nextLine)) {
    ++numLines;
  }

  fileStream.clear();
  fileStream.seekg(0);

  coordinateArray = new Coordinate*[numLines];

  numLines = 0;

  while (getline(fileStream, nextLine)) {
     coordinateArray[numLines] = processLine(nextLine);
     numLines++;
  }
  fileStream.close();
}

Coordinate*
CourseReader::atIndex(int index) {
   if(index < numLines && index >= 0) {
     return coordinateArray[index];
   }
   cout << "Index does not exists" << endl;
   return 0;
}

Coordinate*
CourseReader::next() {
   if( currentLine < numLines && currentLine >= 0) {
     if(currentLine == numLines) {
       return coordinateArray[currentLine];
     } else {
       return coordinateArray[currentLine++];
     }
   }
   cout << "No more next, End of file" << endl;
   return 0;
}

Coordinate*
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

  return new Coordinate(vect.at(0), vect.at(1), vect.at(2), vect.at(3));
}

void
CourseReader::setFileName(const string &fileN) {
  struct stat buffer;
  if((stat (fileN.c_str(), &buffer) == 0)) {
    fileName = fileN;
  } else {
    cout << fileName << " does not exist" << endl;
    exit(-1);
  }
  fileStream.open(fileN.c_str()); //, ifstream::in);
}
