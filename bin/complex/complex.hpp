#ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>
using namespace std;

class Complex
{
private:
	int real;
	int imag;
public:
        //Constructors
	Complex()                     : real( 0 ),      imag( 0 )      { /*print(); cout << "\tconstructed" << endl;*/ }
	Complex( int r, int i )       : real( r ),      imag( i )      { /*print(); cout << "\tconstructed" << endl;*/ }
	Complex( const Complex &c )   : real( c.real ),	imag( c.imag ) { /*print(); cout << "\tconstructed" << endl;*/ }
	
        //Setters
	void setReal( int r ) { real = r; }
	void setImag( int i ) { imag = i; }
	void setComplex( Complex c ) { real = c.real; imag = c.imag; }
 
        //Getters
	int getReal() { return real; }
	int getImag() { return imag; }
	
        //Important Methods
	Complex add( const Complex & ) const;
	Complex sub( const Complex & ) const;

	void print() const;
	
	// OPERATOR OVERLOADING
	Complex operator+( const Complex & c ) const { return add(c); }
	Complex operator-( const Complex & c ) const { return sub(c); }
	Complex operator*( const Complex & c ) const { return mul(c); }
	Complex operator/( const Complex & c ) const { return div(c); }

	~Complex() { /*print(); cout << "\tdeleted" << endl;*/ }
};

#endif
