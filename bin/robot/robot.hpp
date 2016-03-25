#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "gpio.hpp"

void initRobot()
{
   

} //initRobot

void waitForGoSwitch()
{
  while (digitalRead(GO_SWITCH) == OFF)
  {
    cout << "Waiting for go switch..." << endl;
    if(digitalRead(GO_SWITCH) == ON) {
      cout << "The game is afoot!" << endl;
      break;
    }
    sleep(1);
  }
} //waitForGoSwitch()

#endif

