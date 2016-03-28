#ifndef GPIO_HPP
#define GPIO_HPP

#include <wiringPi.h>
#include "gpio.h"

using namespace std;

class Gpio
{
   public:

      //Constructor
      Gpio();

      // Methods
      int readStart()  { return digitalRead(SWITCH_START);  };
      int readSelect() { return digitalRead(SWITCH_SELECT); };
      int readLS1()    { return digitalRead(SWITCH_LS1);    };
      int readLS2()    { return digitalRead(SWITCH_LS2);    };

      void laserOn()  { digitalWrite(LINE_LASER, ON);  };
      void laserOff() { digitalWrite(LINE_LASER, OFF); };
      void redOn()    { turnOffLEDs(); digitalWrite(LED_RED  , ON); };
      void greenOn()  { turnOffLEDs(); digitalWrite(LED_GREEN, ON); };
      void blueOn()   { turnOffLEDs(); digitalWrite(LED_BLUE , ON); };

      void turnOffLEDs()
      {
         digitalWrite(LED_RED  , OFF);
         digitalWrite(LED_GREEN, OFF);
         digitalWrite(LED_BLUE , OFF);

      } //turnOffLEDs

   private:
      void gpioInit();

}; //class Gpio

#endif
