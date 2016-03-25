#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <wiringPi.h>
#include "gpio.h"
#include "gpio.hpp"

using namespace std;

int main()
{

   cout << "Initializing GPIO" << endl;
   gpioInit();

   usleep(500000);
   cout << "Main code entered" << endl;
   digitalWrite(LED_MAIN, ON);

   Gpio gpio;

   gpio.laserOn();

   usleep(500000);
   gpio.redOn();

   usleep(500000);
   gpio.greenOn();

   usleep(500000);
   gpio.blueOn();

   usleep(500000);
   cout << "Start Switch state: " << gpio.readStart() << endl;

   usleep(500000);
   cout << "Select Switch state: " << gpio.readSelect() << endl;

   usleep(500000);
   cout << "Limit Switch 1's state is: " << gpio.readLS1() << endl;

   usleep(500000);
   cout << "Limit Switch 2's state is: " << gpio.readLS2() << endl;

   usleep(500000);
   gpio.laserOff();

   usleep(500000);

   gpio.turnOffLEDs();

   cout << "Main code exiting " << endl;
   digitalWrite(LED_MAIN, OFF); 
   digitalWrite(LED_GPIO, OFF); 
}
