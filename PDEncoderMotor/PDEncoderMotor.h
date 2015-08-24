#ifndef PDEncoderMotor_h
#define PDEncoderMotor_h

#include "Arduino.h"


class PDEncoderMotor
{
  public:
    PDEncoderMotor(int pinA, int pinB, int pinC); 
    
    //void TIMER0_COMPA_vect();
    int degrees_in_wheel_ticks(int);
    int wheel_ticks_in_degrees(int);
    void interpolate_trajectory(void);
    void pd_control(int);
    void drive_motor(int);
    void enableOutput(void);
    void initializeMotor(void);
    void interrupt(void);
    void moveTo(int);
    boolean invertPins(void);
    void setVariables(float, float, float, float, float, float);
    void setCurrentCount(int);
    int getSpeed(void);
    void run(void);
    
    int getGMSticks(void);
    void setGMSticks(int);
    int getGReleasePD(void);
    void setGReleasePD(int);
    
  private:
    int G_ms_ticks;
    int G_release_pd;
    int G_current_speed;
    int G_max_speed;
    int G_previous_counts;
    int G_previous_T;
    int G_pd_frequency;
    int G_absolute_degrees;
    int G_degree_step_size;
    
    int current_counts;
    boolean invertedPins;
    
    int COUNTS_PER_ROTATION;
    
    float G_Pm;
    float G_Pr;
    float G_Vm;
    float G_T;
    float G_Kp;
    float G_Kd;
    
    int pin1;
    int pin2;
    int pin3; 
};

#endif
