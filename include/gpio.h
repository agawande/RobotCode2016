#ifndef GPIO_H
#define GPIO_H

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

#define ID_SD         0
#define SDA           2
#define SCL           3
#define GPCLK0        4
#define LED_GPIO      5    // Defined for robot
#define LED_MAIN      6    // Defined for robot
#define CE1           7
#define CE0           8
#define MISO_A        9
#define MOSI_A        10
#define SCLK_A        11
#define LED_RED       13    // Defined for robot
#define TXD           14
#define RXD           15
#define LINE_LASER    18    // Defined for robot
#define LED_GREEN     19    // Defined for robot
#define SWITCH_START  20    // Defined for robot
#define SWITCH_SELECT 21    // Defined for robot
#define SWITCH_LS1    24    // Defined for robot
#define SWITCH_LS2    23    // Defined for robot
#define LED_BLUE      26    // Defined for robot

#endif
