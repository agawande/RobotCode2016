//Stand input output library for prints and what not
#include <stdio.h>

//Library to help manipute strings
#include <string.h>

//Many different uses such as Random Number Generation and also used for the exit functions
#include <stdlib.h>

//Libraries used for Shared Memory 
#include <sys/ipc.h>
#include <sys/shm.h>


//shmKey is the identifier set to a memory location. This identifier helps other programs search
//for the identifier and use the Shared Memory location. Can be set to any arbitrary number.
key_t shmKey = 60;

//This is the variable where the shared memory will be accessed.
char *shared_memory;

//This is the id used in the creation of the shared memory. The sham id can be used to detach,
//and remove the shared memory.
int shmid;

//***Works, but will eventually need to be changed to store more that 1 previous commands.
//previousCommand stores the previous command sent from the Python program so that
//it can check against the shared memory to see if the shared memory has been updated.
//once the shared memory has been updated, previous command is also updated, and waits again 
//for a new command
char previousCommand[10];

//creates the shared memory to be used by other programs and this one itself
void createSharedMemory()
{	
	printf("\n");
	printf("	Created Shared Memory\n");
	
	//Setup shared memory,
	//Use the shmKey for the memory identifier
	//10 is the size in bytes for how much memory is allocated to max size for a 32 bit
	//integer is 10 digits in decimal.
	//the shared memory.
	//The last parameter sets flags for the memory location, which basically tells
	//who or what can access the memory, and granting read / write access to other programs
	//This grants another program read and write access, as well as this main program.
	if ((shmid = shmget(shmKey, 10, IPC_CREAT | 0x1B6)) < 0)
	{	
		//if anything is returned that is less than zero, and error occured when setting
		//the shared memory up.
		exit(2);
	}
	
}

//Unused function as of now... But instead of creating another shared memory, this
//function can be used to find another shared memory... All that would need to be done
//is set the shmKey to the appropriate value to find another shared memory location that
//has been set up.
void findSomeSharedMemory()
{
	if ((shmid = shmget(shmKey, 10, 0666)) < 0)
	{
		exit(3);
	}

}

//This function attaches a certain variable to the shared memory, so that the information
//from the shared memory can be accessed via the pointer shared_memory.
//This will only work if either the createSharedMemory() or the findSomeSharedMemory()
//functions have been called to get the appropriate shmid. 
void attachToSharedMemory()
{
	// Attached shared memory Using the shmid found from above functions.
	//The NULL pointer in the second argument states that it does not matter where the segment 
	//is saved in memory, and that the system can provide the first available address 
	//for the data to be stored.
	printf("	Attached To Shared Memory\n");
	if ((shared_memory = shmat(shmid, NULL, 0)) == (char *) -1)
	{
		exit(4);
	}
}

//Unused as of now....
void invalidMessage()
{
	printf("		Invalid Command!\n");
	memcpy(shared_memory, "invalidop", sizeof("invalidop"));	
}


//FIRST NESTED WHILE LOOP in the main program!
void checkForNewCommand()
{
	while(1)
	{	
		//continously check to see if the memory has been updated
		//disregard the shared_memory if it contains "completed" since that was
		//sent by this program to alert the main python program that the teensy completed 
		//its command.
		if(strcmp(shared_memory, "completed") != 0)
		{
			//Compared the shared_memory with previous command, if they are equal continue
			//to check until the shared_memory is updated.
			if (((strcmp(shared_memory, previousCommand)) > 0) || ((strcmp(shared_memory, previousCommand)) < 0))
			{	
				//Store the new command in previous command for later checking and break from
				//the nested while loop.
				strcpy(previousCommand, shared_memory);
				break;
			}
		}
	}	
}

//Unused as of now...
void getInitialCommand()
{	
	findSomeSharedMemory();
	attachToSharedMemory();
	strcpy(previousCommand, shared_memory);
}

//Updates the shared_memory once the main program receives a completed response from the
//Teensy, by doing a memory copy.
void completedCommand()
{	
	printf("		Completed Command\n");
	memcpy(shared_memory, "completed", sizeof("completed"));
	printf("		Sent Completion\n");
}


//convert the string received from the shared_memory to an integer, which will then be
//saved as an unsigned int.
int convertStringToInt()
{	
	int dataToSend;
	sscanf(previousCommand, "%d", &dataToSend);

	return dataToSend;
}

   
