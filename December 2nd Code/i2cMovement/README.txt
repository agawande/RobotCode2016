This is to be loaded onto the Teensy 3.1. It uses two L298N H-bridges which require 4 pins and two enable lines each coming from the Teensy each.

This code uses four motors. 
The H-bridge can handle two DC motors, or 1 stepper motor.

One motor will use: 

	With the H-Bridge:
		Two IN pins (e.g. IN1 and IN2) - which controls the direction of the motor (One pin High and the other Low goes one direction, and the opposite for the other direction)
		One EN pin (e.g. ENA) - which controls the speed of the motor through Pulse Width Modulation (0-255)
		Two OUT pins (e.g. OUT1 and OUT2) - which provides the direction with the proper voltage and current for the motor

	With the Teensy 3.1:
		Two pins to provide IN functionality for the H-bridge
		One pin to provide EN functionality for the H-bridge
		Two pins for Encoder line A and Encoder Line B	(Which provide the current location of the motor by using the Encoder Library which has built in interrupts attached to the pin that is assigned. The wires should come from the motor to the Teensy)

The Teensy 3.1 is connected to the Raspberry Pi 2 using the SDA and SCL pins by using i2c (TWI)