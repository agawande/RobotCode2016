//Used for prints and stuff
#include <stdio.h>
//Used for exits and stuff
#include <stdlib.h>

//Used for i2c communications
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

//used for timers and reads/writes from files
#include <unistd.h>

//used for file controls
#include <fcntl.h>

//This is the address that the Raspberry Pi will be accessing. The Teensy 3.1
//is set to this address in its own code.
#define ADDRESS 0x04

// The I2C bus: This is for V2 pi's. For V1 Model B you need i2c-0
static const char *devName = "/dev/i2c-1";

//This is used for setting up the i2c connection
int file;

//This is used to determine the motor states, by storing the received signal from the
//Teensy
int motorState = 1;

//This is used to set up the connection with the Teensy
void setupConnection()
{
	printf("	Setup i2c Connection\n");
	
	//set the dev name for the Pi, and the permissions for reading and writing
	if ((file = open(devName, O_RDWR)) < 0) 
	{
		printf("Error i2c\n");
		exit(4);
	}
 
	//Set i2c device as slave, with the address selected from above
	if (ioctl(file, I2C_SLAVE, ADDRESS) < 0) 
	{
		printf("Error i2c\n");
		exit(5);
	}
}	

//function used to determine the motor state, reads from the Teensy via i2c,
//and either returns a 1 or a 0 which the Teensy provides
void checkMotorState()
{
	char buf[1];
		if (read(file, buf, 1) == 1) 
		{
			motorState = (int) buf[0];
		}
	
}

//This is the function to send the command to the Teensy via i2c
void sendData(unsigned int data)
{	
	//all print cosmetic stuff
	unsigned int printData = data;
	printf("				Sending Data...\n");
	printf("				  %u", data);
	printf(" - 0x%X\n", printData);
	
	
	//this is used to shift through the character array to send the
	//correct alotted bits in the certain bytes, since i2c sends
	//one byte at a time
	int i;
	
	unsigned char cmd[16];

	//loop 4 times, to send 4 bytes containing 8 bits... 32 bits total.
	for(i = 3; i >= 0; i--)
	{
		//Shift through the bits, too send the correct bytes, whatever remains is
		//truncated
		cmd[0] = data >> (8*i);
		
		//Write to cmd to i2c using file
		if (write(file, cmd, 1) == 1) 
		{	
			// As we are not talking to direct hardware but a microcontroller we
			// need to wait a short while so that it can respond.
			printf("				Sending Data %i\n", i);
			printf("				  %s\n", cmd);
			// 1ms seems to be enough but it depends on what workload it has
			usleep(10000);	
		}
	}
	//make sure motor state is one (could probably be removed)
	motorState = 1;
}

//SECOND NESTED WHILE LOOP* is used to check for the completion of the command sent
//to the Teensy 3.1
void checkForCompletion()
{
	while(1)
	{	
		//Read the bus, it should be a 1 or a 0.
		char buf[1];
		if (read(file, buf, 1) == 1) 
		{
			//store the byte into motorState
			motorState = (int) buf[0];
				
			//If 0 is return, Teensy finished tasked, and the loop 
			//can be broken out of, to continue with the proces.
			if (motorState == 0)
			{	
				//cosmetic stuff.
				printf("\n");
				printf("		Received Completion Response\n");
				
				
				break;
			}
		}
	}
	//ensure motorState is 1 again
	motorState = 1;	
}


