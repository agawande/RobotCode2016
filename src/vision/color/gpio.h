#ifndef GPIO_H
#define GPIO_H

#include <wiringPi.h>

/******************************************************************
*       This block is used to setup the GPIO pins on the RPi      *
*       There are 4 sections:                                     *
*        1) PIN ASSIGNMENTS - assign pins for defined functions   *
*        2) PREDEFINED SETUP - these pins have predefined         *
*                functions and are listed for reference purposes  *
*                and are thus commented out.                      *
*        3) USER DEFINED SETUP - these pins are have user defined *
*                functionality.                                   *
*        4) UNDEFINED SETUP - these pins are unused and have a    *
*                default condition imposed on them (GPIO.OUT      *
*                                                                 *
******************************************************************/

#define OFF 0
#define ON  1

/*********************************************************************
*                          PIN LABELS                                *
*                  Using Broadcom Pin Numbers                        *
*********************************************************************/

#define ID_SD       0
#define SDA         2
#define SCL         3
#define GPCLK0      4
#define LED_WHITE   5    // Defined for robot
#define LASER_LINE  6    // Defined for robot
#define CE1         7
#define CE0         8
#define MISO_A      9
#define MOSI_A      10
#define SCLK_A      11
#define LASER_CROSS 12   // LASER_CROSS Defined for robot
#define GO_SWITCH   13
#define COURSE_SW   16
#define LSW_CORNER  17
#define LSW_MIDDLE  22
#define TXD         1
#define RXD         15
#define PCM_C       18
#define MISO_B      19
#define MOSI_B      20
#define SCLK_B      21

void gpioInit();



/******************************************************************
*                         GPIO Initialization                     *
******************************************************************/
void gpioInit(void)
{
   /*********************************************************************
    *                         PREDEFINED SETUP                          *
    *          ONLY CHANGE IF YOU KNOW WHAT YOU ARE DOING!!             *
    *********************************************************************/

   /*
                     !!!  EXTREME WORD OF CAUTION !!
     !!! BY DEFAULT PINS SHOULD BE SET TO INPUTS TO PROTECT FROM SHORTS !!!

     !!! ALL PINS ARE SET TO INPUTS IN THE CASE OF ACCIDENTAL UNCOMMENTING !!!

    */

   // // // pinMode(ID_SD,  INPUT);    // I2C, SHOULD NEVER OVERRIDE !!!!
   // // // pinMode(SDA,    INPUT);      // I2C, SHOULD NEVER OVERRIDE !!!!
   // pinMode(SCL,    INPUT);
   // pinMode(CPCLK0, INPUT);
   // pinMode(CE1,    INPUT);
   // pinMode(CE0,    INPUT);
   // pinMode(MISO_A, INPUT);
   // pinMode(MOSI_A, INPUT);
   // pinMode(SCLK_A, INPUT);
   // pinMode(TXD,    INPUT);
   // pinMode(RXD,    INPUT);
   // pinMode(PCM_C,  INPUT);
   // pinMode(MISO_B, INPUT);
   // pinMode(MOSI_B, INPUT);
   // pinMode(SCLK_B, INPUT);

   /******************************************************************
   *                        UNDEFINED SETUP                          *
   *   -These are only listed to remind us what is available.        *
   *   -They should remain commented out when not in use and         *
   *     removed from this section if used.                          *
   ******************************************************************/

   /*
                     !!!  EXTREME WORD OF CAUTION !!
     !!! BY DEFAULT PINS SHOULD BE SET TO INPUTS TO PROTECT FROM SHORTS !!!

     !!! ALL PINS ARE SET TO INPUTS IN THE CASE OF ACCIDENTAL UNCOMMENTING !!!

    */

   // pinMode(23, INPUT);
   // pinMode(24, INPUT);
   // pinMode(25, INPUT);
   // pinMode(26, INPUT);

   wiringPiSetupGpio(); // Use Broadcom pin numbers

   pinMode(LED_WHITE,   OUTPUT);
   pinMode(LASER_LINE,  OUTPUT);
   pinMode(LASER_CROSS, OUTPUT);

}

#endif
