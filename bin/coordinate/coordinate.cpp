#include <iostream>
#include "coordinate.hpp"

using namespace std;

Coordinate::Coordinate(int index, int x, int y, int orient) {
  setIndex(index);
  setX(x);
  setY(y);
  setOrient(orient);
}

void Coordinate::setIndex(int index) {
  this->index = index;
}

void Coordinate::setX(int x) {
  this->x = x;
}


void Coordinate::setY(int y) {
  this->y = y;
}

void Coordinate::setOrient(int orient) {
  this->orient = orient;
}

void Coordinate::print() {
  if (this != 0) {
    cout << this->getIndex() << ", " << this->getX() << ", " << this->getY() << ", " << this->getOrient() << endl;
  } else {
    cout << "null pointer" <<  endl;
  }
}

//Coordinate &add //return by value
