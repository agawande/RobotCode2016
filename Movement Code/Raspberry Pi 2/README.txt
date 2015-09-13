To operate the code:

First run the main communication program, this creates the shared memory allocation
and begins the i2c communication. (The Teensy 3.1 code can be ran at anytime however)

Then start the python mock up testing program, from here, 4 byte commands can be 
sent through the shared memory to MainCommunication program. Which will then be
sent through i2c to the Teensy 3.1.

Note: The commands have to follow the Specific format given in the MessageFormatter header 
file. This format is also explain in the Teensy 3.1 code (PDEncoderMotorCode file)
above the moveHere method. Basic commands are given in the text document called i2c basic
commands. 

This file gives formatted messages for all commands that can be given to the motors, with
max distances.

Note: The main python mock up program will crash if the same command is sent two times 
in a row. This is intentional because the same command should never be sent two times in a row
because of the way the keys are set up in the message formatter.
