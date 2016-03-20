#ifndef COMPLEX_H
#define COMPLEX_H

class Complex {

private:
  //Imaginary part and
  //real part
  int x, y, index, orient;

public:
  Complex( int , int , int, int );

  void setX( int );
  void setY( int );
  void setIndex( int );
  void setOrient( int );

  int getX() const { return x; }
  int getY() const { return y; }
  int getIndex() const { return index; }
  int getOrient() const { return orient; }

  //These functions do the operation as
  //var = calle.operation(argument)
  //ex: c = a.add(b)

  void print();

}; //End of complex class

#endif
