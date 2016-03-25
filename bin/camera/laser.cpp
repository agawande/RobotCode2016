#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <wiringPi.h>
#include "gpio.h"
#include "gpio.hpp"

using namespace std;

int main(int argc, char *argv[])
{

   gpioInit();

   Gpio gpio;

   
   (argc == 1) ? gpio.laserOff() : gpio.laserOn();

   cout << "argc: " << argc << endl;
}
