#include <PDEncoderMotor.h>                                      //Library used to calculate the proportional and derivative control from the motors
                                                                 //using an interpolated trajectory
#include <Encoder.h>                                             //Library Used for reading Quaddrature Encoders (Teensy based library)

#include <Wire.h>

//Set the Address for the Slave (Teensy 3.1)
#define SLAVE_ADDRESS 0x04

byte dataReceived[] = {0, 0, 0, 0};                             //This array of Bytes is where the incoming data from the Pi will be stored.


int state;                                                      //This variable is used to keep track of the state of the Motors. When the Motors are in Standby Mode waiting for a command and while they
                                                                //are moving, state is written to 1. When the motors reach their destination and the distance to move is 0, state is written to 0,
                                                                //for a very brief amount of time, (1 milisecond) and in this time, the Pi picks up that the command is completed. Once the Pi sends the 
                                                                //command to the Teensy 3.1, the Pi will wait in a while(1) loop, repeatedly checking the i2c bus to see if the Teensy has completed its operation.
                                                                //once the state is 0 for 1 ms, the Pi picks this up, and then sends a completed response to the Main Python program, through shared memory.


int numberOfByteReceived = 0;                                   //This keeps track of the number order of the Byte that has been collected for i2c (since i2c sends only one byte at a time), so that the bytes will remaining in 
                                                                //a consistent order coming from the received transmission from the Pi

int PD_FREQUENCY = 1000;                                        //The rate at which the timer runs for the 
                                                                //interrupts to function correctly
                                                                //This 1000 hertz timer is used in the Pd
                                                                //Control to calculate the speed for the next
                                                                //iteration

int pX1[5] = {16, 15, 23, 0, 1};                                //Pins used for each motor:
int pX2[5] = {14, 13, 22, 2, 3};                                //Direction A, Direction B, PWM (Speed), Encoder A, Encoder B
int pY1[5] = {11, 12, 20, 6, 7};                                //is the order of the array
int pY2[5] = {9, 10, 21, 4, 5};

Encoder X1Encoder(pX1[3], pX1[4]);                              //The Encoder Library attaches an interrupt to the pins selected, runs much faster using pins. This aids in keeping accurate tick counts for the
Encoder X2Encoder(pX2[3], pX2[4]);                              //position, without taking time away from the Timer interrupts used for the PD control
Encoder Y1Encoder(pY1[3], pY1[4]);
Encoder Y2Encoder(pY2[3], pY2[4]);

long normalizeX1, normalizeX2, normalizeY1, normalizeY2;       
long normalizedX1, normalizedX2, normalizedY1, normalizedY2;     
long newPosY1, newPosY2, newPosX1, newPosX2;                    //Variables used to store the New Encoder tick counts when the .read() operation is called

int difference = 0;                                             //Variable used to find the difference count between two Motor encoder positions
int lowerSpeed = 0;                                             //A speed variable used when lowering the speed of a certain motor, used when synchronizing
int raiseSpeed = 0;                                             //This variable is used for increasing the speed of a certain motor, used when synchronizing

IntervalTimer X1Timer;                                          //This IntervalTimer class sets up an interrupt timer for a 1000 hz. Everytime the interrupt runs, it calls a function which sends data to the PD
IntervalTimer X2Timer;                                          //control system, to give a new speed and calculate trajectory.
IntervalTimer Y1Timer;
IntervalTimer Y2Timer;

PDEncoderMotor X1(pX1[0], pX1[1], pX1[2]);                      //This creates a new PDEncoderMotor object, with the appropriate pins, PD functionallity is contains in this class' library
PDEncoderMotor X2(pX2[0], pX2[1], pX2[2]); 
PDEncoderMotor Y1(pY1[0], pY1[1], pY1[2]); 
PDEncoderMotor Y2(pY2[0], pY2[1], pY2[2]); 

float kp = 22;                                                //All of these values are used in the PDEncoder library, kp is the proportional control, kd is the derivative
float kd = 1;                                                  //Step size is measured in degrees, less accurate with higher values, will increase speed with higher values.
float stepSize = 50;                                          
float pm = 0;                                                   //These are position values used in the calculate, no values needed to be run (optional for testing purposes)
float pr = 0;
float vm = 0;                                                   //Initial velocity values (optional setting)

//Kp - Tested 17-14 works ok KD tested 0.09-0.55, works ok
float kpAllWheel = 16.75;                                       //All of these values are used in the PDEncoder library, kp is the proportional control, kd is the derivative
float kdAllWheel = .1;                                        //Step size is measured in degrees, less accurate with higher values, will increase speed with higher values.
float stepSizeAllWheel = 40;                                          
float pmAllWheel = 0;                                           //These are position values used in the calculate, no values needed to be run (optional for testing purposes)
float prAllWheel = 0;
float vmAllWheel = 0;

int targetPositionX = 0;                                        //This is the position to go when a move command is sent
int targetPositionY = 0;                                        //This is the position to go when a move command is sent

boolean beenReachedX = false;                                   //Used to see if the X axis motors reached position
boolean beenReachedY = false;                                   //Used to see if the X axis motors reached position

float synchSpeed = 2;                                           //This variable determines how much to speed up and slow down motors when synchronizing, the lower the values, the less the difference between the two
                                                                //encoder positions matters, high values, slower, more accurate
int minimumMoveSpeed = 5;

boolean commandFinished = false;
boolean allWheelCommand = false;
  
void setup()
{
  //Serial.begin(9600);
  //Serial.println("Hello");
  //delay(5000);
  Wire.begin(SLAVE_ADDRESS);                                     //Start the I2C Bus as Slave on address
  //Serial.println("Hello");
  //delay(5000);
  Wire.onReceive(receiveEvent);                                  //Attach a function to trigger when something is received.

  Wire.onRequest(sendData);                                      //Whenever the Master (Pi) sends a signal to the Slave (Teensy)
                                                                 //The receieveEvent function is called immediately, and that code is ran.
                                                                 //Attach a function to trigger when Data is requested from the Master.
                                                                 //Whenever the Master (Pi) sends a signal to the Slave (Teensy)
                                                                 //The receieveEvent function is called immediately, and that code is ran.
                                                                 
  X1.setVariables(kp, kd, stepSize, pm, pr, vm);                 //Sets the initial value settings for each motor
  X2.setVariables(kp, kd, stepSize, pm, pr, vm);
  Y1.setVariables(kp, kd, stepSize, pm, pr, vm);
  Y2.setVariables(kp, kd, stepSize, pm, pr, vm);
  
  X2.invertPins();                                                //This is two swap direction pins for a motor, Since the motors on the same axis spin in opposite directions relative to their shafts, the direction pins are inverted
  Y1.invertPins();                                                //so that the two motors spin in the same direction, when a move called is made
  
  X1.initializeMotor();                                           //initializes each motor
  X2.initializeMotor();
  Y1.initializeMotor();
  Y2.initializeMotor();
   
  X1Timer.begin(interruptX1, 1000);                               //set up interrupt timer at 1000 microseconds (1 milisecond or 1000 hertz)
  X2Timer.begin(interruptX2, 1000);                               //interrupts functions are used every time the timer reaches 1000 seconds,
  Y1Timer.begin(interruptY1, 1000);                               //interrupts send timing information to the PD control system.
  Y2Timer.begin(interruptY2, 1000);

}


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!MAIN LOOP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void loop()
{ 
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //targetPosition = 149550;                                        //TESTING COMMANDS AND TARGETPOSITIONS
  //spin(targetPosition);
  
  //targetPosition = 50000;
  
  //moveQuadrantI(targetPosition);
  //moveQuadrantII(targetPosition);
  //moveQuadrantIII(targetPosition);
  //moveQuadrantIV(targetPosition);

  //if (newPosY2 == 10000 && time2 == 0)
  //{
  //  time2 = millis(); 
  //  Serial.println((time2 - time1));
  //}
  
  //moveX(targetPosition);
  //moveY(targetPosition);

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  //make State 1
  state = 1;
  
  //Whenever the distance to go reaches zero for both motors
  //(whenever a previous move had been called, the Accelstepper
  //library keeps track of the distance to go)
  //then send out a transmission to the Master signaling that
  //the current move function is complete
  if ((distanceToGoX() == 0 && distanceToGoY() == 0) && (isPositionReachedX() || isPositionReachedY()))
  {
     //This variable is used to keep track of the state of the Motors. When the Motors are in Standby Mode waiting for a command and while they
     //are moving, state is written to 1. When the motors reach their destination and the distance to move is 0, state is written to 0,
     //for a very brief amount of time, (1 milisecond) and in this time, the Pi picks up that the command is completed. Once the Pi sends the 
     //command to the Teensy 3.1, the Pi will wait in a while(1) loop, repeatedly checking the i2c bus to see if the Teensy has completed its operation.
     //once the state is 0 for 1 ms, the Pi picks this up, and then sends a completed response to the Main Python program, through shared memory.
     state = 0;
     beenReachedX = false;
     beenReachedY = false;
     //Delay for 1 milisecond to give the Pi enough time to pick up this state (aid in the i2c communication)
     //Whenever the Pi attempts to read the i2c line from the slave, that sends a request via i2c to the Teensy, when the Teensy receives a request,
     //the Teensy calls the sendData() functions, as shown from the Wire.onRequest(sendData); function from above. This 1 milisecond delay gives plenty of time for the
     //Raspberry Pi 2 to pick up that the move operation has been completed.
     delay(1);
  }
  //Return back to state 1
  state = 1;
  //The runAll function is repeatedly ran, this function branches out to the rest of the functions
  runAll();  
  //adaptValues();  
}
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!MAIN LOOP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



//Whenever the Teensy receieves a signal from the Master, this is ran.
//The function is immediately called and basically reads the four bytes
//that were sent form the master. 
void receiveEvent (int numBytes)
{
    Serial.println(numBytes);
    //When the wire is available, read the four Bytes
    //that were transmitted from the Master.
    while(Wire.available())
    {   
      //Serial.println(Wire.available());
      //Each read() removes a byte from the available bytes.
      //So it starts with four bytes, once read() is called
      //one byte is removed, and the second one is available to 
      //be read. These bytes are stored in dataReceived[0] and dataReceived[1] and so on...
      int temp = Wire.read();
      
      //These if statements check to see if the order of the bytes are correct with respect to the formatting used showed in the comment section of the moveHere() function. Once a byte is received with a ordering key
      //of 0x00 and the number of bytes that has been received and stored is zero, then add this new byte to the dataReceived array and 
      //increment the numberOfByteReceived by one, to indicate one byte has been received. This continues down the if statement tree until the array is full, with a full i2c message.
      if (((temp & 0xC0) == 0x00) && numberOfByteReceived == 0)
      {
        dataReceived[numberOfByteReceived] = temp;
        Serial.println("||||||||||||||||||||||||||||||||||||||||||||");
        Serial.println("******************");
        Serial.println("Data Received:");
        Serial.print("   ");
        Serial.print(dataReceived[numberOfByteReceived]);
        Serial.print(" - ");
        Serial.println(dataReceived[numberOfByteReceived], HEX);
        numberOfByteReceived++;                                                                       //increment the global variable to keep track of the number of the byte being read from the line
      }
        else if (((temp & 0xC0) == 0x40) && numberOfByteReceived == 1)
        {
          dataReceived[numberOfByteReceived] = temp;
          Serial.print("   ");
          Serial.print(dataReceived[numberOfByteReceived]);
          Serial.print(" - ");
          Serial.println(dataReceived[numberOfByteReceived], HEX);
          numberOfByteReceived++;                                                                     //increment the global variable to keep track of the number of the byte being read from the line         
        }
        else if (((temp & 0xC0) == 0x80) && numberOfByteReceived == 2)
        {
          dataReceived[numberOfByteReceived] = temp;
          Serial.print("   ");
          Serial.print(dataReceived[numberOfByteReceived]);
          Serial.print(" - ");
          Serial.println(dataReceived[numberOfByteReceived], HEX);
          numberOfByteReceived++;                                                                     //increment the global variable to keep track of the number of the byte being read from the line
        }
        else if (((temp & 0xC0) == 0xC0) && numberOfByteReceived == 3)
        {
          dataReceived[numberOfByteReceived] = temp;
          Serial.print("   ");
          Serial.print(dataReceived[numberOfByteReceived]);
          Serial.print(" - ");
          Serial.println(dataReceived[numberOfByteReceived], HEX);
          numberOfByteReceived++;                                                                    //increment the global variable to keep track of the number of the byte being read from the line         
        }
      //keep state at 1, can be removed
      state = 1;
   } 

    //Robust Check
    //This huge if statement checks the Keys and Ordering of the 4 Bytes Received, this is explained below in the moveHere function, but basically if they are not in order based on the
    //ordering bits, and if the bytes do not contain the same keys, then the Ordering and the Keys do not match, and an error is sent.
    if ((((dataReceived[0] & 0xC0) == 0x00) && ((dataReceived[1] & 0xC0) == 0x40) && ((dataReceived[2] & 0xC0) == 0x80) && ((dataReceived[3] & 0xC0) == 0xC0)) && ((((dataReceived[0] & 0x30) == (dataReceived[1] & 0x30)) == ((dataReceived[2] & 0x30) == (dataReceived[3] & 0x30))))) 
    {
        Serial.println(" ~Keys and Ordering match!~");
        Serial.println("******************");
        Serial.println(" ");
        Serial.println("Initiating Move...");
        Serial.println("  -----------------------------");
        Serial.print(" | Current X Coordinate ");
        Serial.print(X1.getGAbsolute());
        Serial.println("  ");
        Serial.print(" | Current Y Coordinate ");
        Serial.print(Y2.getGAbsolute());
        Serial.println("  ");
        Serial.println("  -----------------------------");
        
        normalizeX1 = X1Encoder.read();                                      //Uses to normalize all the encoder values to zero. This is used for synchronization only.
        normalizeX2 = X2Encoder.read();
        normalizeY1 = Y1Encoder.read();
        normalizeY2 = Y2Encoder.read();
        
        moveHere();
        dataReceived[0] = 0;                                                 //Reset data received back to zero for next data set
        dataReceived[1] = 0;
        dataReceived[2] = 0;
        dataReceived[3] = 0;
        numberOfByteReceived = 0;
        commandFinished = true;                                              //Command is decoded and ready to be performed, used to keep track of when waiting for a command
    }
      else
      {  
          //Wire.write(state);
          //Serial.println("Keys or Ordering of Bits to dont match");
      }
    
 }   
    
void moveHere()
{    
    //break down the four bytes in the usuable data that is 
    //designated for commands and data and also keys and ordering bits
    
    //The whole four Bytes:
    //00000000 00000000 00000000 00000000
    //Ordering Bits are:
    //00xxxxxx 01xxxxxx 10xxxxxx 11xxxxxx *This is how a correctly ordered 4 bytes should be received on the teensy
    //Key Bits are:
    //xx11xxxx xx11xxxx xx11xxxx xx11xxxx *This is how a correctly keyed 4 bytes should be received, with a key of either 00, 01, 10, or 11 contained in all 4 bytes
    //Commands are:
    //xxxx0000 xxxxxxxx xxxxxxxx xxxxxxxx *Where there are 15 avaible commands contained in these four consequtive bits (as of now, only 4 command types are programmed into the Teensy code)
    //Data are (Distances and Angles):
    //xxxxxxxx xxxx0000 xxxx0000 xxxx0000 *This is how the data is formulated. There are 12 designated bits for the Data (Which is either a distance or and angle based on the commands) 12 bits contain 2^12 or 
    //                                    *4095 possible different data options, which if milimeters are being used in distances, gives 4.095 meters, which is plently for course   
    
    //where x's are dont cares


      //extract the distance by applying a mask of:
      //00001111
      //To the last three bytes of the data array, and then shift each one to their appropriate location
      //Result in a distance where the bits in dataReceived[1] are the Most Significant Bits.
      int dist = ((0x0F & dataReceived[1]) << 8) | ((0x0F & dataReceived[2]) << 4) | (0x0F & dataReceived[3]);
      
    
      double distance = getMM(dist);
      //Extract the directions by the same process of using a mask on the first byte, and using an AND operation.
      byte directions = dataReceived[0] & 0x0F;
      
      Serial.println("Command Decoded:");
      
      //Now use the direction extracted determine how the motors should move
      //Forward: 0x00 -> xxxx0000 
      if (directions == 0x00)
      {
        Serial.print("  Move North: "); 
        Serial.print(directions);
        Serial.print(" Distance: ");
        Serial.print(distance);
        Serial.println("");
        targetPositionY = distance + Y1.getGAbsolute();                        //Uses one motor to keep track of coordinates, uses this same motor to set up a target position relative to current position
        setAllVariablesTwoWheel();                                             //Sets the values for the PD class for only two motors, these variables are used in the Torque equation
        allWheelCommand = false;                                               //This is a two wheel command
        moveY(distance);                                                       //The rest of the commands follow the same general form
      } 
      // Reverse: 0x01 -> xxxx0001 
        else if (directions == 0x01)
        {
          Serial.print("  Move South: "); 
          Serial.print(directions);
          Serial.print(" Distance: ");
          Serial.print(distance);
          Serial.println("");
          targetPositionY = -distance + Y1.getGAbsolute();
          setAllVariablesTwoWheel();
          allWheelCommand = false;
          moveY(-distance);
        }
       // Move Left: 0x02 -> xxxx0010
        else if (directions == 0x02)
        {
          Serial.print("  Move West: "); 
          Serial.print(directions);
          Serial.print(" Distance: ");
          Serial.print(distance);
          Serial.println("");
          targetPositionX = -distance + X2.getGAbsolute();
          setAllVariablesTwoWheel();
          allWheelCommand = false;
          moveX(-distance);     
        }
       // Move Right: 0x03 -> xxxx0011
        else if (directions == 0x03)
        {
          Serial.print("  Move East: "); 
          Serial.print(directions);
          Serial.print(" Distance: ");
          Serial.print(distance);
          Serial.println("");
          targetPositionX = distance + X2.getGAbsolute();
          setAllVariablesTwoWheel();
          allWheelCommand = false;
          moveX(distance);
        }
        // Clockwise spin: 0x04 -> xxxx0100 
        else if (directions == 0x04)
        {
          Serial.print("  Clockwise Spin: "); 
          Serial.print(directions);
          Serial.print(" Distance: ");
          Serial.print(distance);
          Serial.println("");
          targetPositionX = distance + X2.getGAbsolute();
          targetPositionY = distance + Y1.getGAbsolute();
          
          setAllVariablesFourWheel();
          allWheelCommand = true;
          
          spin(distance);
        }
        // Anti Clockwise spin: 0x05 -> xxxx0101 
        else if (directions == 0x05)
        {
          Serial.print("  Anti-Clockwise Spin: "); 
          Serial.print(directions);
          Serial.print(" Distance: ");
          Serial.print(distance);
          Serial.println("");
          
          targetPositionX = -distance + X2.getGAbsolute();
          targetPositionY = -distance + Y1.getGAbsolute();

          setAllVariablesFourWheel();
          allWheelCommand = true;
          
          spin(-distance);
        }
        // Move Quadrant 1: 0x06 -> xxxx0110
        else if (directions == 0x06)
        {
          Serial.print("  Quadrant I Movement: "); 
          Serial.print(directions);
          Serial.print(" Distance: ");
          Serial.print(distance);
          Serial.println("");
          targetPositionX = distance + X2.getGAbsolute();
          targetPositionY = distance + Y1.getGAbsolute();

          setAllVariablesFourWheel();
          allWheelCommand = true;
          
          moveQuadrantI(distance);
        }
        // Move Quadrant II: 0x07 -> xxxx0111
        else if (directions == 0x07)
        {
          Serial.print("  Quadrant II Movement: "); 
          Serial.print(directions);
          Serial.print(" Distance: ");
          Serial.print(distance);
          Serial.println("");
          targetPositionX = -distance + X2.getGAbsolute();
          targetPositionY = distance + Y1.getGAbsolute();
          
          setAllVariablesFourWheel();
          allWheelCommand = true;
          
          moveQuadrantII(distance);
        }
        // Move Quadrant III: 0x08 -> xxxx1000
        else if (directions == 0x08)
        {
          Serial.print("  Quadrant III Movement: "); 
          Serial.print(directions);
          Serial.print(" Distance: ");
          Serial.print(distance);
          Serial.println("");
          
          targetPositionX = -distance + X2.getGAbsolute();
          targetPositionY = -distance + Y1.getGAbsolute();
          
          setAllVariablesFourWheel();
          allWheelCommand = true;
          
          moveQuadrantIII(distance);
        }
        // Move Quadrant IV: 0x09 -> xxxx1001
        else if (directions == 0x09)
        {
          Serial.print("  Quadrant IV Movement: "); 
          Serial.print(directions);
          Serial.print(" Distance: ");
          Serial.print(distance);
          Serial.println("");
          
          targetPositionX = distance + X2.getGAbsolute();
          targetPositionY = -distance + Y1.getGAbsolute();
          
          setAllVariablesFourWheel();
          allWheelCommand = true;
          
          moveQuadrantIV(distance);
        }
        
        Serial.println(" ");
        Serial.println(" ????????????????????????????????????");
        Serial.println(" ? Waiting for completed command... ?");
        Serial.println(" ????????????????????????????????????");
        Serial.println(" ");
}

//sendData is called when the Raspberry Pi requests some data from the Teensy. This is performed mainly when the Raspberry Pi has sent a command to the Teensy 3.1
//and is awaiting confirmation of completed command by the Teensy 3.1. Each request from the Pi triggers this, until the Wire.write(state); command returns a value of 0
//from state, signaling that the motors have completed their operations
void sendData()
{  
  //writes a 0 to the i2c bus
   Wire.write(state);
   //Cosmetic printing, the state 0 should be active for around 1 milisecond, so print the beginning for a new command cosmeticly
   if (state == 0 && commandFinished)
   {
       Serial.println("Completed Command!");
       Serial.println("  -----------------------------");
       Serial.print(" | Current X Coordinate ");
       Serial.print(X1.getGAbsolute());
       Serial.println(" ");
       Serial.print(" | Current Y Coordinate ");
       Serial.print(Y2.getGAbsolute());
       Serial.println(" ");
       Serial.println("  -----------------------------");
       Serial.println("||||||||||||||||||||||||||||||||||||||||||||");
       Serial.println(" ");

       commandFinished = false;                                   //Reset these back to false, until a new command is received and decoded
       allWheelCommand = false;    
   }
   
   //reset the numberOfByteReceived to 0, so that the bytes can remain in order.
   if (numberOfByteReceived == 4)
   {
      numberOfByteReceived = 0;
   }    
}

void runAll()                                                    //The run all functions contains all of the commands to move the motors, update the PD control, and get a new encoder position for each motor
{
  newPosY1 = Y1Encoder.read();                                   //This block of code reads all of the current positions on each motor encoder
  newPosY2 = Y2Encoder.read();
  newPosX1 = X1Encoder.read();
  newPosX2 = X2Encoder.read();
  
  
  
  Y2.run();
  Y2.setCurrentCount(newPosY2);
  X1.run();                                                      //This runs the motor, comments regarding this function is in the PDEncoder library
  X1.setCurrentCount(newPosX1);                                  //setCurrentCount updates the PD system on the current motor position
  X2.run();
  X2.setCurrentCount(newPosX2);
  Y1.run();
  Y1.setCurrentCount(newPosY1);

  
  if (((abs(distanceToGoX()) > 20) || (abs(distanceToGoY()) > 20)) && commandFinished == true)
  { 
    //Serial.println("Synching");
    synchronize();                                                                                               //This function synchronizes the X and Y axises, and then synchronizes A motor from the X and with a motor from the Y axis
  }
}

void setAllVariablesTwoWheel()
{
  X1.setVariables(kp, kd, stepSize, pm, pr, vm);                                                                 //Sets the initial value settings for each motor for Two wheel movement commands
  X2.setVariables(kp, kd, stepSize, pm, pr, vm);
  Y1.setVariables(kp, kd, stepSize, pm, pr, vm);
  Y2.setVariables(kp, kd, stepSize, pm, pr, vm); 
}

void setAllVariablesFourWheel()
{
  X1.setVariables(kpAllWheel, kdAllWheel, stepSizeAllWheel, pmAllWheel, prAllWheel, vmAllWheel);                 //Sets the initial value settings for four wheel motor commands
  X2.setVariables(kpAllWheel, kdAllWheel, stepSizeAllWheel, pmAllWheel, prAllWheel, vmAllWheel);
  Y1.setVariables(kpAllWheel, kdAllWheel, stepSizeAllWheel, pmAllWheel, prAllWheel, vmAllWheel);
  Y2.setVariables(kpAllWheel, kdAllWheel, stepSizeAllWheel, pmAllWheel, prAllWheel, vmAllWheel); 
}

void spin(int distance)                                                                                          //Spins the robot clockwise or counter clockwise depending on the distance used 
{
  X1.moveTo(-distance);
  X2.moveTo(distance);
  Y1.moveTo(distance);
  Y2.moveTo(-distance);
}

void moveX(int distance)
{ 
  X1.moveTo(distance);
  X2.moveTo(distance);
}

void moveY(int distance)
{ 
  Y1.moveTo(distance);
  Y2.moveTo(distance);
}

void moveQuadrantI(int distance)
{
  X1.moveTo(distance);
  X2.moveTo(distance);
  Y1.moveTo(distance);
  Y2.moveTo(distance);                                             
}

void moveQuadrantII(int distance)                                 //The rest of the movement commands are self explanatory
{
  X1.moveTo(-distance);
  X2.moveTo(-distance);
  Y1.moveTo(distance);
  Y2.moveTo(distance);                                            
}

void moveQuadrantIII(int distance)
{
  X1.moveTo(-distance);
  X2.moveTo(-distance);
  Y1.moveTo(-distance);
  Y2.moveTo(-distance);
}

void moveQuadrantIV(int distance)
{
  X1.moveTo(distance);
  X2.moveTo(distance);
  Y1.moveTo(-distance);
  Y2.moveTo(-distance);
}

int distanceToGoX(void)                                                   //Determines the distance to go on the X axis
{                                                                         //returns the distance to be used
  return (abs(targetPositionX/5) - abs(newPosX2));   
}

int distanceToGoY(void)                                                   //Determines the distance to go on the Y axis
{
  return (abs(targetPositionY/5) - abs(newPosY1));
}

boolean isPositionReachedX(void)                                         //Determines if the position is reached
{                                                                        //Returns a boolean for checking with other functions
  if (distanceToGoX() == 0  && beenReachedX == false)
  {
    beenReachedX = true;
    return true;
  } 
   return false; 
}

boolean isPositionReachedY(void)
{
  if (distanceToGoY() == 0  && beenReachedY == false)
  {
    beenReachedY = true;
    return true;
  } 
  return false; 
}

//*****************************************************************************************************************
//*****************************************************************************************************************
//These are not actually interrupts, just functions that are called when the Interval Timer interrupt is activated. These  
//functions are called from the TimerInterval objects that were instantiated from above. The interrupt built in the
//TimerInterval class calls the respective functions ever 1000 microseconds, when the interrupts activates.
void interruptX1(void) 
{

  int temp = X1.getGMSticks();                              //This updates the current tick of the encoder, it receives the previous tick from the specific class, updates it with 1 more tick, and then sends it back to the
  temp++;                                                   //PD System.
  X1.setGMSticks(temp);                              
  if ((int)(temp % (1000 / PD_FREQUENCY)) == 0) 
  {
    X1.setGReleasePD(1);
  }
}
  
void interruptX2(void) 
{

  int temp = X2.getGMSticks();
  temp++;
  X2.setGMSticks(temp);
  if ((int)(temp % (1000 / PD_FREQUENCY)) == 0) 
  {
    X2.setGReleasePD(1);
  }
}

void interruptY1(void) 
{

  int temp = Y1.getGMSticks();
  temp++;
  Y1.setGMSticks(temp);
  if ((int)(temp % (1000 / PD_FREQUENCY)) == 0) 
  {
    Y1.setGReleasePD(1);
  }
}

void interruptY2(void) 
{

  int temp = Y2.getGMSticks();
  temp++;
  Y2.setGMSticks(temp);
  if ((int)(temp % (1000 / PD_FREQUENCY)) == 0) 
  {
     Y2.setGReleasePD(1);
  }
}
//*****************************************************************************************************************
//*****************************************************************************************************************

//All of the synchronize functions will do a check between only two motors at a time, for example SynchronizeY checks the absolute value  of the encoder counts of the Y axis motors, if one is greater than the other, then slow the
//motor with the higher counts down by the difference between the two encoder counter and then divide by the synchSpeed, which is set by the user. The motor with the lower counts also has its speed increased by the difference, divided
//by the synchSpeed. The lower the synch speed goes, the faster the two motors will run. This synchronizing methodology is used for the each synchronize function. Synchonize All functions just synchronizes the X and Y axis, while
//the X and Y axises are being synched as well using synchronizeX and synchronizeY.

void synchronize(void)                                        
{    
   synchronizeX();                                      //Same thing as above, just with the X axis        
   synchronizeY();                                      //Synchronizes the Y axis
   if (allWheelCommand == true)
   {
      synchronizeAll();                                 //This synchronizes the X1 motor with Y2 motor, and the X2 motor with the Y1 motor                                        
   }
}

void synchronizeX()
{
  normalizedX1  = abs(abs(newPosX1) - abs(normalizeX1));
  normalizedX2  = abs(abs(newPosX2) - abs(normalizeX2));

  if (abs(normalizedX1) > abs(normalizedX2))
  {
    difference = abs(abs(normalizedX1) - abs(normalizedX2));
    raiseSpeed = abs(X1.getSpeed());    
    
    if ((raiseSpeed - (difference/synchSpeed)) < 0)
    {
      lowerSpeed = 0; 
    }
    else
    {
      lowerSpeed  = abs(abs(raiseSpeed) - (difference/synchSpeed));
      lowerSpeed  = abs(map(lowerSpeed, 0, 255, minimumMoveSpeed, 255));
    }

    analogWrite(pX2[2], (raiseSpeed + (difference/synchSpeed)));
    analogWrite(pX1[2], (lowerSpeed));
  }      
  if (abs(normalizedX2) > abs(normalizedX1))
  {
    difference = abs(abs(normalizedX2) - abs(normalizedX1));
    raiseSpeed = abs(X2.getSpeed()); 

    if ((raiseSpeed - (difference/synchSpeed)) < 0)
    {
      lowerSpeed = 0; 
    }
    else
    {
      lowerSpeed  = abs(abs(raiseSpeed) - (difference/synchSpeed));
      lowerSpeed  = abs(map(lowerSpeed, 0, 255, minimumMoveSpeed, 255));
    }

    analogWrite(pX1[2], (raiseSpeed + abs(difference/synchSpeed)));
    analogWrite(pX2[2], (lowerSpeed));
  }  
}

void synchronizeY()
{
  normalizedY1  = abs(abs(newPosY1) - abs(normalizeY1));
  normalizedY2  = abs(abs(newPosY2) - abs(normalizeY2));

  if (abs(normalizedY1) > abs(normalizedY2))
  {
    difference = abs(abs(normalizedY1) - abs(normalizedY2));
    raiseSpeed = abs(Y1.getSpeed());    
    
    if ((raiseSpeed - (difference/synchSpeed)) < 0)
    {
      lowerSpeed = 0; 
    }
    else
    {
      lowerSpeed  = abs(abs(raiseSpeed) - (difference/synchSpeed));
      lowerSpeed  = abs(map(lowerSpeed, 0, 255, minimumMoveSpeed, 255));
    }

    analogWrite(pY2[2], (raiseSpeed + (difference/synchSpeed)));
    analogWrite(pY1[2], (lowerSpeed));
  }      
  if (abs(normalizedY2) > abs(normalizedY1))
  {
    difference = abs(abs(normalizedY2) - abs(normalizedY1));
    raiseSpeed = abs(Y2.getSpeed()); 

    if ((raiseSpeed - (difference/synchSpeed)) < 0)
    {
      lowerSpeed = 0; 
    }
    else
    {
      lowerSpeed  = abs(abs(raiseSpeed) - (difference/synchSpeed));
      lowerSpeed  = abs(map(lowerSpeed, 0, 255, minimumMoveSpeed, 255));
    }

    analogWrite(pY1[2], (raiseSpeed + abs(difference/synchSpeed)));
    analogWrite(pY2[2], (lowerSpeed));
  }  
}

void synchronizeAll()
{
  normalizedX1  = abs(abs(newPosX1) - abs(normalizeX1));
  normalizedX2  = abs(abs(newPosX2) - abs(normalizeX2));
  normalizedY1  = abs(abs(newPosY1) - abs(normalizeY1));
  normalizedY2  = abs(abs(newPosY2) - abs(normalizeY2));
  
  
  if (abs(normalizedX1) > abs(normalizedY1))
  {
    difference = abs(abs(normalizedX1) - abs(normalizedY1));
    raiseSpeed = abs(X1.getSpeed());    
    
    if ((raiseSpeed - (difference/synchSpeed)) < 0)
    {
      lowerSpeed = 0; 
    }
    else
    {
      lowerSpeed  = abs(abs(raiseSpeed) - (difference/synchSpeed));
      lowerSpeed  = abs(map(lowerSpeed, 0, 255, minimumMoveSpeed, 255));
    }

    analogWrite(pY1[2], (raiseSpeed + (difference/synchSpeed)));
    analogWrite(pX1[2], (lowerSpeed));
  }    
  
  if (abs(normalizedY1) > abs(normalizedX1))
  {
    difference = abs(abs(normalizedY1) - abs(normalizedX1));
    raiseSpeed = abs(Y1.getSpeed()); 

    if ((raiseSpeed - (difference/synchSpeed)) < 0)
    {
      lowerSpeed = 0; 
    }
    else
    {
      lowerSpeed  = abs(abs(raiseSpeed) - (difference/synchSpeed));
      lowerSpeed  = abs(map(lowerSpeed, 0, 255, minimumMoveSpeed, 255));
    }

    analogWrite(pX1[2], (raiseSpeed + abs(difference/synchSpeed)));
    analogWrite(pY1[2], (lowerSpeed));
  }  
    
  if (abs(normalizedY2) > abs(normalizedX2))
  {
    difference = abs(abs(normalizedY2) - abs(normalizedX2));
    raiseSpeed = abs(Y2.getSpeed());    
    
    if ((raiseSpeed - (difference/synchSpeed)) < 0)
    {
      lowerSpeed = 0; 
    }
    else
    {
      lowerSpeed  = abs(abs(raiseSpeed) - (difference/synchSpeed));
      lowerSpeed  = abs(map(lowerSpeed, 0, 255, minimumMoveSpeed, 255));
    }

    analogWrite(pX2[2], (raiseSpeed + (difference/synchSpeed)));
    analogWrite(pY2[2], (lowerSpeed));
  }    
  
  if (abs(normalizedX2) > abs(normalizedY2))
  {
    difference = abs(abs(normalizedX2) - abs(normalizedY2));
    raiseSpeed = abs(X2.getSpeed()); 

    if ((raiseSpeed - (difference/synchSpeed)) < 0)
    {
      lowerSpeed = 0; 
    }
    else
    {
      lowerSpeed  = abs(abs(raiseSpeed) - (difference/synchSpeed));
      lowerSpeed  = abs(map(lowerSpeed, 0, 255, minimumMoveSpeed, 255));
    }

    analogWrite(pY2[2], (raiseSpeed + abs(difference/synchSpeed)));
    analogWrite(pX2[2], (lowerSpeed));
  } 
}
//
//void adaptValues()
//{
//    if(distanceToGoX() > 0 && distanceToGoX() < 2001)
//    {
//      float tempkp;
//      float tempkd;
//      float tempStepSize;
//      
//      tempkd = (((distanceToGoX() - 2000)*8)/19990) + 1;
//      tempkp = (((distanceToGoX() - 2000)*7)/1999) + 22;
//      tempStepSize =  (((distanceToGoX() - 2000) * 35)/1999) + 50;
//      
//      
//        X1.setVariables(tempkp, tempkd, tempStepSize , pm, pr, vm);                 //Sets the initial value settings for each motor
//        X2.setVariables(tempkp, tempkd, tempStepSize , pm, pr, vm);
//        
//            Serial.println(tempkp);
//    Serial.println(tempkd);
//    Serial.println(tempStepSize);
//
//    }
//      else if (distanceToGoY() > 0 && distanceToGoY() < 2001) 
//      {
//        float tempkp;
//        float tempkd;
//        float tempStepSize;
//      
//        tempkd = (((distanceToGoY() - 2000)*8)/19990) + 1;
//        tempkp = (((distanceToGoY() - 2000)*7)/1999) + 22;
//        tempStepSize =  (((distanceToGoY() - 2000) * 35)/1999) + 50;
//  
//        Y1.setVariables(tempkp, tempkd, tempStepSize , pm, pr, vm);
//        Y2.setVariables(tempkp, tempkd, tempStepSize , pm, pr, vm);
//        
//            Serial.println(tempkp);
//            Serial.println(tempkd);
//            Serial.println(tempStepSize);
//        
//      }
//      else if (((distanceToGoX() > 0) || (distanceToGoY() > 0)) && distanceToGoY() < 2001)
//      {
//        float tempkp;
//        float tempkd;
//        float tempStepSize;
//        
//      
//        tempkd = (((distanceToGoY() - 2000)*8)/19990) + 1;
//        tempkp = (((distanceToGoY() - 2000)*7)/1999) + 22;
//        tempStepSize =  (((distanceToGoY() - 2000) * 35)/1999) + 50;
//        
//        X1.setVariables(tempkp, tempkd, tempStepSize , pm, pr, vm);                 //Sets the initial value settings for each motor
//        X2.setVariables(tempkp, tempkd, tempStepSize , pm, pr, vm);
//        Y1.setVariables(tempkp, tempkd, tempStepSize , pm, pr, vm);
//        Y2.setVariables(tempkp, tempkd, tempStepSize , pm, pr, vm);
//        
//            Serial.println(tempkp);
//    Serial.println(tempkd);
//    Serial.println(tempStepSize);
//      }
//
//}

double getMM(int distance){
  //22400 because it takes 4480 counts per revolution and *10 because Derek said so
  //d = 70.29 mm
  return (distance*22400)/220.8225476208265667;
}
