#include <wiringPiI2C.h>

//include "i2cInterface.h"

int main ()
{
   int fd = 0;
   //Required to initialize wiringPi
   int wiringPiSetupGpio();

   //Required to initialize I2C
   //int wiringPiI2CSetup ( MOTOR_DRIVER_ADDRESS );
   fd =  wiringPiI2CSetup ( 25 );

   printf("fd = %d\n",fd);

   unsigned int directionByte = 1; // forward
   unsigned int dist1Byte = 75; 
   unsigned int dist2Byte = 139;
   unsigned int dist3Byte = 203;

   int crap = 0;

   int i = 0;
   

   //Let's just loop around for testing purposes
   while ( i < 5 )
   {
      
      crap = wiringPiI2CWrite ( fd, directionByte );
      delay(1000);
      crap = wiringPiI2CWrite ( fd, dist1Byte );
      delay(1000);
      crap = wiringPiI2CWrite ( fd, dist2Byte );
      delay(1000);
      crap = wiringPiI2CWrite ( fd, dist3Byte );

      // Send Distance Nibbles starting from most significant values
      

      //Sleep for a predefined amount of time
      void delay ( SLEEP_TIME );

      i++;
   }

   return 1;
}
