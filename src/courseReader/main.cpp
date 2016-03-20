#include "course_reader.h"
#include <iostream>
#include "complex.h"

using namespace std;

int main () {
   string fileName = "hello";
   CourseReader cR(fileName);

   (cR.next()).print();
   (cR.next()).print();
   (cR.next()).print();

   (cR.atIndex(1)).print();
}
