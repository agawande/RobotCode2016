#include "Complex.hpp"

Complex Complex::add( const Complex &c ) const
{
	Complex result;
	result.real = real + c.real;
	result.imag = imag + c.imag;
	return result;
}

Complex Complex::sub( const Complex &c ) const
{
	Complex result;
	result.real = real - c.real;
	result.imag = imag - c.imag;
	return result;
}

void Complex::print() const { cout << "( " << real << ", " << imag << " )"; }
