#ifndef COMPLEX_H
#define COMPLEX_H

class Coordinate {

  private:
     //Imaginary part and
     //real part
     int index, x, y, orient;

  public:

     //Constructors
     Coordinate( int, int ,int , int );

     //Public methods
     void setX( int );
     void setY( int );
     void setIndex( int );
     void setOrient( int );

     int getX() const { return x; }
     int getY() const { return y; }
     int getIndex() const { return index; }
     int getOrient() const { return orient; }

     void print();

}; //End of complex class

#endif
