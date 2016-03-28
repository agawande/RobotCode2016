#include "coursereader.hpp"
#include <iostream>
#include "complex.hpp"

using namespace std;

int main () {
   const string fileName = "hello";

   CourseReader cR;
   cR.setFileName(fileName);
   cR.readFileIntoArray();

   (cR.next())->print();
   (cR.next())->print();
   (cR.next())->print();
   (cR.next())->print();

   (cR.atIndex(1))->print();

   cR.atIndex(3)->print();
}
