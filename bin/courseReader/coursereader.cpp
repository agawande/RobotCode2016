#include "coursereader.hpp"
#include "complex.hpp"
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
  delete complexArray;
}

void
CourseReader::readFileIntoArray()
{
  while (getline(fileStream, nextLine)) {
    ++numLines;
  }

  fileStream.clear();
  fileStream.seekg(0);

  complexArray = new Complex*[numLines];

  numLines = 0;

  while (getline(fileStream, nextLine)) {
     complexArray[numLines] = processLine(nextLine);
     numLines++;
  }
  fileStream.close();
}

Complex*
CourseReader::atIndex(int index) {
   if(index < numLines && index >= 0) {
     return complexArray[index];
   }
   cout << "Index does not exists" << endl;
   return 0;
}

Complex*
CourseReader::next() {
   if( currentLine < numLines && currentLine >= 0) {
     if(currentLine == numLines) {
       return complexArray[currentLine];
     } else {
       return complexArray[currentLine++];
     }
   }
   cout << "No more next, End of file" << endl;
   return 0;
}

Complex*
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

  return new Complex(vect.at(0), vect.at(1), vect.at(2), vect.at(3));
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
