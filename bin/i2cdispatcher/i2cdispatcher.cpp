#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#include <unistd.h>
#include <fcntl.h>

#include "i2cdispatcher.hpp"

using namespace std;

int I2cDispatcher::getFile()
{ 
   return file; 
} //getFile()


void I2cDispatcher::setupConnection()
{
   std::cout << "I2C connection setup to address " << i2cAddress <<endl;

   //set the dev name for the Pi, and the permissions for reading and writing
   if ((file = open(devName, O_RDWR)) < 0)
   {
     cerr << "Error I2C" << endl;
     exit(1);
   }

   //Set i2c device as slave, with the address selected from above
   if (ioctl(file, I2C_SLAVE, i2cAddress) < 0)
   {
     cerr << "Error I2C: Device not found at" << i2cAddress << endl;
     exit(1);
   }
} //setupConnection()


//This is the function to send the command to the Teensy via i2c
void I2cDispatcher::sendData(unsigned int data)
{
   //this is used to shift through the character array to send the
   //correct alotted bits in the certain bytes, since i2c sends
   //one byte at a time
   int i;

   unsigned char cmd[16];

   //loop 4 times, to send 4 bytes each containing 8 bits... 32 bits total.
   for(i = 3; i >= 0; i--)
   {
     //Shift through the bits, too send the correct bytes, whatever remains is
     //truncated
     cmd[0] = data >> (8*i);

     //Write to cmd to i2c using file
     if (write(file, cmd, 1) == 1)
     {
         // As we are not talking to direct hardware but a microcontroller we
         // need to wait a short while so that it can respond.
         std::cout << "Sending Data: " << i <<  ", Command: " << cmd << endl;

         // 1ms seems to be enough but it depends on what workload it has
         usleep(10000);
      }
   }
} //sendData()
