The Sorting operation uses:

Teensy 3.2 overclocked at 144Mhz

The Teensy is overclocked and the highclock speed is needed because it has to produce an appropriate 
amount of Steps for three stepper motors at the same time. The X Y and Z axis motors are running 
with a 1/8 Microstep rate. This means that instead of 1.8 degrees per step or 200 steps for revolution, 
it is running 0.225 degrees per step or 1600 steps per revolution. To get the speed needed for the Sorting
operation (which needs to be fast) the stepper motors need to move at around 400-500 rpm. This means
the Teensy needs to produce 720000 steps per minute, for three motors.

Three DRV 8825 Stepper motor drivers are used. All that is needed as inputs for motor control is a 
Step Input, and a Direction input. The Step input is controlled by the AccelStepper library, using 
the properties mentioned above.

!!! Use the library in the folder, since it has been modified!!!