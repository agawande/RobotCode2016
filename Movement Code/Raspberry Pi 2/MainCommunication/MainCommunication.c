
//include the Libraries specifically made for handling the
//Shared Memory and i2c functions for the robot
//messageFormater.h is probably be used in the main python code, which will send the data
//to this C program

#include <SharedMemoryInterface.h> 
#include <MessageFormatter.h>
#include <i2cInterface.h>

//This setup functions calls all the initially needed functions to get i2c
//and shared memory up and running, their names are pretty much self explantory
//more detail contained in the header libraries

void setup()
{	
	createSharedMemory();
	attachToSharedMemory();
	
	//This initially setups of the previousCommand variable, so that the previous
	//command can be checked with the shared_memory, to verify a duplicate command
	//isn't being sent. This previousCommand variable will probably need to be exapnded
	//so that multiple previous commands can be stored in case of an emergency situation
	strcpy(previousCommand, shared_memory);
	
	//sets up i2c connection
	setupConnection();
}

int main()
{	
	//cosmetic prints
	printf("Beginning Motor Initialization and Communication\n");
	
	setup();
	
	//This is a continuously checking while loop, which checks for a new command to be sent
	//from the main Python program continuously in another while loop. 
	//Its essentially waits until the shared_memory has been updated
	
	//Once the shared_memory is updated, the program then sends the data to the via i2c
	//to the Teensy 3.1. While the Teensy is running there is another function which
	//contains a while loop which checks the Teensy via i2c to see if the command has been 
	//completed. 
	
	//Once the Teensy completes it command, the Teensy responds with completion signal.
	//The signal is then sent to the Python program via Shared Memory.
	//Once the shared memory is updated, the Python again waits in another while loop
	//waiting for the shared memory to be updated....
	//This process continually happens and does not end until termination or failure? lol
	while(1)
	{	
		//cosmetic prints
		printf("\n");
		printf("		Waiting For Command...\n");
		printf("\n");
		
		//FIRST NESTED WHILE LOOP*
		//Initiate nested while(1) loop to check for updated shared memory
		checkForNewCommand();
		
		//Once here, a new command has been received from the shared memory
		printf("			Received Command %s\n" , previousCommand);
		
		//This data comes as a string, and needs to be converted to an unsigned int
		//to be sent approriately to the i2c, which is expecting 4 bytes.
		// 2 ^32 is the value we need, which is an unsigned int.
		unsigned int formattedData = convertStringToInt();
		
		//send the formatted data via i2c
		sendData(formattedData);
		printf("		Data Sent!\n");
		printf("\n");	
		
		//SECOND NESTED WHILE LOOP*
		//Initiate nested while(1) loop to check for completed command from Teensy
		checkForCompletion();

		//Update the shared_memory to show that the Teensy has completed the command
		//and awaits a new command.
		completedCommand();
	}
	
   return(0);
}






