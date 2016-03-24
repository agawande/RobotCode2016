#include <iostream>
#include "complex.h"

using namespace std;

Complex::Complex(int index, int x, int y, int orient) {
  setIndex(index);
  setX(x);
  setY(y);
  setOrient(orient);
}

void Complex::setIndex(int index) {
  this->index = index;
}

void Complex::setX(int x) {
  this->x = x;
}


void Complex::setY(int y) {
  this->y = y;
}

void Complex::setOrient(int orient) {
  this->orient = orient;
}

void Complex::print() {
  if (this != 0) {
    cout << this->getIndex() << ", " << this->getX() << ", " << this->getY() << ", " << this->getOrient() << endl;
  } else {
    cout << "null pointer" <<  endl;
  }
}

//complex &add //return by value
