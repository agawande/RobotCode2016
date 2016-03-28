#include "gpio.hpp"

Gpio::Gpio()
{
   gpioInit();
} //Gpio


/******************************************************************
*                         GPIO Initialization                     *
******************************************************************/
void Gpio::gpioInit(void)
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
   
   // pinMode(13, INPUT);
   // pinMode(16, INPUT);
   // pinMode(17, INPUT);
   // pinMode(22, INPUT);
   // pinMode(23, INPUT);
   // pinMode(24, INPUT);
   // pinMode(25, INPUT);
   // pinMode(26, INPUT);
   
   wiringPiSetupGpio(); // Use Broadcom pin numbers
   
   pinMode(LINE_LASER ,  OUTPUT);
   pinMode(LED_GPIO   ,  OUTPUT);
   pinMode(LED_MAIN   ,  OUTPUT);
   pinMode(LED_RED    ,  OUTPUT);
   pinMode(LED_GREEN  ,  OUTPUT);
   pinMode(LED_BLUE   ,  OUTPUT);

   pinMode(SWITCH_START , INPUT);
   pinMode(SWITCH_SELECT, INPUT);
   pinMode(SWITCH_LS1   , INPUT);
   pinMode(SWITCH_LS2   , INPUT);

   pullUpDnControl(SWITCH_START , PUD_DOWN);
   pullUpDnControl(SWITCH_SELECT, PUD_DOWN);
   pullUpDnControl(SWITCH_LS1   , PUD_DOWN);
   pullUpDnControl(SWITCH_LS2   , PUD_DOWN);

   digitalWrite(LED_GPIO, ON);

} //gpioInit
