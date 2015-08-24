#include "Arduino.h"
#include "PDEncoderMotor.h"
#include "Encoder.h"

PDEncoderMotor::PDEncoderMotor(int pinA, int pinB, int pinC)
{
  pin1 = pinA;
  pin2 = pinB;
  pin3 = pinC;

  
  G_ms_ticks = 0;
  G_release_pd = 0;
  G_current_speed = 0;
  G_max_speed = 0;
  G_previous_counts = 0;
  G_previous_T = 0;
  G_pd_frequency = 1000;
  G_absolute_degrees = 0;
  G_degree_step_size = 0;
  
  current_counts = 0;
  
  COUNTS_PER_ROTATION = 64;
  
  invertedPins = false;
  
  G_Pm = 0;
  G_Pr = 0;
  G_Vm = 0;
  G_T = 0;
  G_Kp = 0;
  G_Kd = 0;  
}

int PDEncoderMotor::getSpeed(void)
{
  return G_T;
}

void PDEncoderMotor::moveTo(int distance)
{
    G_absolute_degrees = distance;
}

boolean PDEncoderMotor::invertPins(void)
{
    int temp = pin1;
    pin1 = pin2;
    pin2 = temp;
    invertedPins = true;
    return  invertedPins;
}

void PDEncoderMotor::setVariables(float kp, float kd, float stepSize, float pm, float pr, float vm)
{
  G_Kp = kp;
  G_Kd = kd;
  G_degree_step_size = stepSize;
  G_Pm = pm;
  G_Pr = pr; 
  G_Vm = vm;
  
}

void PDEncoderMotor::setCurrentCount(int count)
{
   current_counts = count; 
}

void PDEncoderMotor::initializeMotor(void)
{  
   enableOutput();
//   IntervalTimer myTimer;
//   IntervalTimer myTimer;
//   myTimer.begin(PDEncoderMotor::interrupt, 1000);
   
}

void PDEncoderMotor::run(void)
{
  if (G_release_pd) 
  {
    G_release_pd = 0;
    //execute pd controller
    interpolate_trajectory();
   }  
  
}
void PDEncoderMotor::enableOutput(void)
{
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
}

int PDEncoderMotor::getGMSticks(void)
{
   return G_ms_ticks; 
}

void PDEncoderMotor::setGMSticks(int set)
{
   G_ms_ticks = set; 
}

int PDEncoderMotor::getGReleasePD(void)
{
   return G_release_pd; 
}

void PDEncoderMotor::setGReleasePD(int set)
{
   G_release_pd = set; 
}

int PDEncoderMotor::degrees_in_wheel_ticks(int degrees) 
{
  return (degrees / (360 / COUNTS_PER_ROTATION));
}

int PDEncoderMotor::wheel_ticks_in_degrees(int wheel_ticks) 
{
  return (wheel_ticks * (360 / COUNTS_PER_ROTATION));
}

void PDEncoderMotor::interpolate_trajectory(void)
{
  // G_Pm = current counts
  int target;
  int pm_degrees = wheel_ticks_in_degrees(G_Pm);
  
  if (G_absolute_degrees >= pm_degrees) {
    int findMin = (G_absolute_degrees - pm_degrees);
    findMin = min(G_degree_step_size, findMin);
    target = pm_degrees + findMin;
  }
  else {
    int findMin = (pm_degrees - G_absolute_degrees);
    findMin = min(G_degree_step_size, findMin);
    target = pm_degrees - findMin;
  }
  pd_control(target);
}

void PDEncoderMotor::pd_control(int relative_degrees) 
{
  // T = Kp(Pr - Pm) - Kd*Vm
  //
  // T = Output motor signal (torque)
  // Pr = Desired motor position
  // Pm = Current motor position
  // Vm = Current motor velocity (computed based on finite differences)
  // Kp = Proportional gain
  // Kd = Derivative gain

  //Encoder myEncoder(pin4, pin5);
  
  //current_counts = myEncoder.read();//encoders_get_counts_m2();
  
  //int check = wheel_ticks_in_degrees(current_counts);
  if (invertedPins == true)
  {
     current_counts = (current_counts*(-1)); 
  }

  int temp_Vm;

  G_Pm = current_counts;
  G_Pr = degrees_in_wheel_ticks(relative_degrees);

  G_Vm = G_current_speed;
  // G_Vm = (G_ms_ticks * current_counts) - (G_ms_ticks * G_previous_counts);

  G_T = (G_Kp * (G_Pr - G_Pm)) - (int)(((float)G_Kd/10.0f) * (float)G_Vm);

  drive_motor(G_T);

  // every 10ms


  if (G_T > G_max_speed) 
  {
    G_max_speed = G_T;
  }

  G_current_speed = G_T;
  G_previous_counts = current_counts;

}

void PDEncoderMotor::drive_motor(int speed) 
{
  
  int pwm_top = 255;
  int speedtest = (abs)(speed); 
  speedtest = map(speedtest, 0, 255, 36, 255); 
  
  if (abs(speed) < 255) 
  {
    pwm_top = abs(speed);
  }

  // stop the motor
  if (speed == 0) 
  {
    // data direction for motor port = input
   digitalWrite(pin1, LOW);
   digitalWrite(pin2, LOW);
   analogWrite(pin3, speedtest);
  }
  else 
  {
    // data direction for motor port = output
    if (speed > 0) 
    {
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, HIGH);
      analogWrite(pin3, abs(speedtest));
    }
    else if (speed < 0)
    {
      digitalWrite(pin1, HIGH);
      digitalWrite(pin2, LOW);
      analogWrite(pin3, abs(speedtest));
    }
  }
}
