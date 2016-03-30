//Include Encoder and PID_v1 to enable DCMotorServo's usage of them.
//(see: http://stackoverflow.com/questions/6504211/is-it-possible-to-include-a-library-from-another-library-using-the-arduino-ide)
#include <Encoder.h>
#include "PID_v1.h"
#include "DCMotorServo.h"
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

//(2*pi/360)*6*2.54 mm
const float ANGLE_CONVER = 2.65988;
int whichSync;

//X1 (DirB, DirA, PWM, ENC B, ENC A), to get X Axis in same direction.
DCMotorServo X1 = DCMotorServo(21, 22, 20, 15, 14);
//X2 (DirA, DirB, PWM, ENC A, ENC B), can stay default
DCMotorServo X2 = DCMotorServo(6, 7, 9, 8, 10);
//Y1 (DirB, DirA, PWM, ENC B, ENC A), to get Y Axis in same direction.
DCMotorServo Y1 = DCMotorServo(2, 4, 5, 12, 11);
//Y2 (DirA, DirB, PWM, ENC A, ENC B), can stay default
DCMotorServo Y2 = DCMotorServo(0, 1, 3, 17, 16);
int counter = 0;

long normalizeX1, normalizeX2, normalizeY1, normalizeY2;       
long normalizedX1, normalizedX2, normalizedY1, normalizedY2;    

boolean beenReached = false;
boolean commandFinished = false;

void setup() 
{
  Wire.begin(SLAVE_ADDRESS);                                     //Start the I2C Bus as Slave on address
  //Serial.println("Hello");
  //delay(5000);
  Wire.onReceive(receiveEvent);                                  //Attach a function to trigger when something is received.

  Wire.onRequest(sendData);                                      //Whenever the Master (Pi) sends a signal to the Slave (Teensy)
                                                                 //The receieveEvent function is called immediately, and that code is ran.
                                                                 //Attach a function to trigger when Data is requested from the Master.
                                                                 //Whenever the Master (Pi) sends a signal to the Slave (Teensy)
                                                                 //The receieveEvent function is called immediately, and that code is ran.
  
  Serial.begin(9600);
  
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  
  //Tune the servo feedback
  //Determined by trial and error
  //servo.myPID->SetTunings(0.1,0.15,0.05);
  X1.myPID->SetTunings(0.1,0.15,0.1);
  X1.setPWMSkip(40);
  X1.setAccuracy(3);
  
  X2.myPID->SetTunings(0.1,0.15,0.1);
  X2.setPWMSkip(40);
  X2.setAccuracy(3);
  
  Y1.myPID->SetTunings(0.1,0.15,0.1);
  Y1.setPWMSkip(40);
  Y1.setAccuracy(3);
//  
  Y2.myPID->SetTunings(0.1,0.15,0.1);
  Y2.setPWMSkip(40);
  Y2.setAccuracy(3);
  
  X1.myPID->SetResolution(1);
  X2.myPID->SetResolution(1);
  Y1.myPID->SetResolution(1);
  Y2.myPID->SetResolution(1);
  
  X1.myPID->SetSampleTime(250);
  X2.myPID->SetSampleTime(250);
  Y1.myPID->SetSampleTime(250);
  Y2.myPID->SetSampleTime(250);
  
  X1.myPID->SetTunings(0.15,0.2,0.30);
  X2.myPID->SetTunings(0.15,0.2,0.30);
  Y1.myPID->SetTunings(0.15,0.2,0.30);
  Y2.myPID->SetTunings(0.15,0.2,0.30);
  analogWriteFrequency(20, 46875);
  analogWriteFrequency(9, 46875);
  analogWriteFrequency(5, 46875);
  analogWriteFrequency(3, 46875);
  analogWriteResolution(8);
  
  
  whichSync = 2;
  
  moveNorth(10000);
}


void loop() 
{
  //make State 1
  state = 1;


Serial.println(X1.getActualPosition());
Serial.println(X2.getActualPosition());
Serial.println(Y1.getActualPosition());
Serial.println(Y2.getActualPosition());

  if ((X1.distanceToGo() < 200) && (X2.distanceToGo() < 200) && (Y1.distanceToGo() < 200) && (Y2.distanceToGo() < 200))
  {
      X1.myPID->SetTunings(0.01,5,4);
      X2.myPID->SetTunings(0.01,5,4);
      Y1.myPID->SetTunings(0.01,5,4);
      Y2.myPID->SetTunings(0.01,5,4);
  }
  else if ((X1.distanceToGo() < 400) && (X2.distanceToGo() < 400) && (Y1.distanceToGo() < 400) && (Y2.distanceToGo() < 400))
  {
      X1.myPID->SetTunings(0.3,3,4);
      X2.myPID->SetTunings(0.3,3,4);
      Y1.myPID->SetTunings(0.3,3,4);
      Y2.myPID->SetTunings(0.3,3,4);
  }
  else
  {
      
      X1.myPID->SetTunings(1,0.2,0.15);
      X2.myPID->SetTunings(1,0.2,0.15);
      Y1.myPID->SetTunings(1,0.2,0.15);
      Y2.myPID->SetTunings(1,0.2,0.15);
  }
//   if ((X1.distanceToGo() < 800) && (X2.distanceToGo() < 800) && (Y1.distanceToGo() < 800) && (Y2.distanceToGo() < 800))
//  {
//      X1.myPID->SetTunings(.05,1,.9);
//      X2.myPID->SetTunings(.05,1,.9);
//      Y1.myPID->SetTunings(.05,1,.9);
//      Y2.myPID->SetTunings(.05,1,.9);
//  }
//  else if ((X1.distanceToGo() < 1800) && (X2.distanceToGo() < 1800) && (Y1.distanceToGo() < 1800) && (Y2.distanceToGo() < 1800))
//  {
//      X1.myPID->SetTunings(.1,.5,.7);
//      X2.myPID->SetTunings(.1,.5,.7);
//      Y1.myPID->SetTunings(.1,.5,.7);
//      Y2.myPID->SetTunings(.1,.5,.7);
//  }
//  else
//  {
//      
//      X1.myPID->SetTunings(.3,0.2,0.15);
//      X2.myPID->SetTunings(.3,0.2,0.15);
//      Y1.myPID->SetTunings(.3,0.2,0.15);
//      Y2.myPID->SetTunings(.3,0.2,0.15);
//  }     
  
//      X1.myPID->SetTunings(.4,0.1,0.4);
//      X2.myPID->SetTunings(.4,0.1,0.4);
//      Y1.myPID->SetTunings(.4,0.1,0.4);
//      Y2.myPID->SetTunings(.4,0.1,0.4);
//      if (whichSync == 0)
//      {
//        synchronizeTwo(X1, X2);
//      }
//      else if (whichSync == 1)
//      {
//        synchronizeTwo(Y2, Y1);
//      }
//      else
//      {
        //synchronizeTwo(X1, X2);
        synchronizeTwo(Y2, Y1);
        //synchronizeTwo(X2, Y2);
        //synchronizeTwo(Y1, X1);
//      }
//  }



    
  //Whenever the distance to go reaches zero for both motors
  //(whenever a previous move had been called, the Accelstepper
  //library keeps track of the distance to go)
  //then send out a transmission to the Master signaling that
  //the current move function is complete
  if (X1.finished() && X2.finished() && Y1.finished() && Y2.finished()) 
  {
      //stop disengages the motor feedback system, in which case if you moved the motor, it would fight you if you didn't "stop" it first (provided you were still running servo.run() operations)
      X1.stop();
      X2.stop();
      Y1.stop();
      Y2.stop();
      
      X1.setCurrentPosition(0);
      X2.setCurrentPosition(0);
      Y1.setCurrentPosition(0);
      Y2.setCurrentPosition(0);
      
      X1.setSetPoint(0);
      X2.setSetPoint(0);
      Y1.setSetPoint(0);
      Y2.setSetPoint(0);
  
       //This variable is used to keep track of the state of the Motors. When the Motors are in Standby Mode waiting for a command and while they
       //are moving, state is written to 1. When the motors reach their destination and the distance to move is 0, state is written to 0,
       //for a very brief amount of time, (1 milisecond) and in this time, the Pi picks up that the command is completed. Once the Pi sends the 
       //command to the Teensy 3.1, the Pi will wait in a while(1) loop, repeatedly checking the i2c bus to see if the Teensy has completed its operation.
       //once the state is 0 for 1 ms, the Pi picks this up, and then sends a completed response to the Main Python program, through shared memory.
       state = 0;
       beenReached = false;

       //Delay for 1 milisecond to give the Pi enough time to pick up this state (aid in the i2c communication)
       //Whenever the Pi attempts to read the i2c line from the slave, that sends a request via i2c to the Teensy, when the Teensy receives a request,
       //the Teensy calls the sendData() functions, as shown from the Wire.onRequest(sendData); function from above. This 1 milisecond delay gives plenty of time for the
       //Raspberry Pi 2 to pick up that the move operation has been completed.
       delay(1);
  }
//    else if(motor_timeout < millis()) {
//      //Setting a move operation will ensure that servo.finished() no longer returns true
//        delay(500);
//        motor_go = 1;
//        if (counter == 0)
//        {
//          Serial.println(Y1.getActualPosition());
//          Serial.println(Y2.getActualPosition());
//          normalizeX1 = 0;
//          normalizeX2 = 0;
//          moveY(7000);
//          counter++;
//        }
//
//        else if (counter == 1)
//        {
//          Serial.println(X1.getActualPosition());
//          Serial.println(X2.getActualPosition());
//          normalizeX1 = 0;
//          normalizeX2 = 0;
//          moveX(7000);
//          counter++;
//        }
//        
//       else if (counter == 2)
//        {
//          Serial.println(Y1.getActualPosition());
//          Serial.println(Y2.getActualPosition());
//          normalizeX1 = 7000;
//          normalizeX2 = 7000;
//          moveY(-7000);
//          counter++;
//        }
//        else if (counter == 3)
//        {
//          Serial.println(X1.getActualPosition());
//          Serial.println(X2.getActualPosition());
//          normalizeX1 = 7000;
//          normalizeX2 = 7000;
//          moveX(-7000);
//          counter = 0;
//          //spin(16000);
//          //counter++;
//        }
//       else if (counter == 4)
//        {
//          moveX(-4000);
//          counter = 0;
//        }

  //delay(1);
    //Return back to state 1
    state = 1;
    
    X1.run();
    X2.run();
    Y1.run();
    Y2.run();
}

void receiveEvent (int numBytes)
{
    Serial.println("Received" + numBytes);
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
        Serial.print(X1.getActualPosition());
        Serial.println("  ");
        Serial.print(" | Current Y Coordinate ");
        Serial.print(Y2.getActualPosition());
        Serial.println("  ");
        Serial.println("  -----------------------------");

        
        X1.setEncoderNormalize();                                      //Uses to normalize all the encoder values to zero. This is used for synchronization only.
        X2.setEncoderNormalize();
        Y1.setEncoderNormalize();
        Y2.setEncoderNormalize();
        
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
      
    
      int distance = getMM(dist);
      //Extract the directions by the same process of using a mask on the first byte, and using an AND operation.
      byte directions = dataReceived[0] & 0x0F;
      
      Serial.println("Command Decoded:");
      
      
         switch(directions) 
         {
           case 0: whichSync = 1; moveNorth(distance); break;
           case 1: whichSync = 1; moveSouth(distance); break;
           case 2: whichSync = 0; moveWest(distance); break;
           case 3: whichSync = 0; moveEast(distance); break;
           case 4: whichSync = 2; distance = getMM(ANGLE_CONVER*dist); spin(distance); break;
           case 5: whichSync = 2; distance = getMM(ANGLE_CONVER*dist); spin(-distance); break;
           case 6: whichSync = 2; moveQuadrantI(distance); break;
           case 7: whichSync = 2; moveQuadrantII(distance); break;
           case 8: whichSync = 2; moveQuadrantIII(distance); break;
           case 9: whichSync = 2; moveQuadrantIV(distance); break;
           case 10: goThroughTunnel0();break;
           case 11: goThroughTunnel1();break;
         }
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
       Serial.print(X1.getActualPosition());
       Serial.println(" ");
       Serial.print(" | Current Y Coordinate ");
       Serial.print(Y2.getActualPosition());
       Serial.println(" ");
       Serial.println("  -----------------------------");
       Serial.println("||||||||||||||||||||||||||||||||||||||||||||");
       Serial.println(" ");

       commandFinished = false;                                   //Reset these back to false, until a new command is received and decoded
   }
   
   //reset the numberOfByteReceived to 0, so that the bytes can remain in order.
   if (numberOfByteReceived == 4)
   {
      numberOfByteReceived = 0;
   }    
}


void synchronizeTwo(DCMotorServo a, DCMotorServo b)
{  
   if (a.finished() || b.finished())
   {  
     return;
   }
   
   int minimumMoveSpeed = 0;
   int synchSpeed = 1.9;
   int difference = 0;
   int raiseSpeed = 0; 
   int lowerSpeed = 0;
   
   
   int normalizedA  =  abs(abs(a.getActualPosition()  - abs(a.getEncoderNormalize())));//abs(abs(newPosX1) - abs(normalizeX1));
   int normalizedB  =  abs(abs(b.getActualPosition()  - abs(b.getEncoderNormalize()))); //abs(abs(newPosX2) - abs(normalizeX2));

  if (abs(normalizedA) > abs(normalizedB))
  {
    difference = abs(abs(normalizedA) - abs(normalizedB));
    raiseSpeed = abs(a.getCurrentPWM());    
    
    if ((raiseSpeed - (difference/synchSpeed)) < 0)
    {
      lowerSpeed = 0; 
    }
    else
    {
      lowerSpeed  = abs(abs(raiseSpeed) - (difference/synchSpeed));
      lowerSpeed  = abs(map(lowerSpeed, 0, 255, minimumMoveSpeed, 255));
    }

    analogWrite(b.getPWMPin(), (raiseSpeed + (difference/synchSpeed)));
    analogWrite(a.getPWMPin(), (lowerSpeed));
  }      
  if (abs(normalizedB) > abs(normalizedA))
  {
    difference = abs(abs(normalizedB) - abs(normalizedA));
    raiseSpeed = abs(b.getCurrentPWM()); 

    if ((raiseSpeed - (difference/synchSpeed)) < 0)
    {
      lowerSpeed = 0; 
    }
    else
    {
      lowerSpeed  = abs(abs(raiseSpeed) - (difference/synchSpeed));
      lowerSpeed  = abs(map(lowerSpeed, 0, 255, minimumMoveSpeed, 255));
    }

    analogWrite(a.getPWMPin(), (raiseSpeed + abs(difference/synchSpeed)));
    analogWrite(b.getPWMPin(), (lowerSpeed));
  }  
} 
  
void spin(int distance)                                                                                          //Spins the robot clockwise or counter clockwise depending on the distance used 
{
  X1.move(-distance);
  X2.move(-distance);
  Y1.move(distance);
  Y2.move(-distance);
}

void moveWest(int distance)
{ 
  X1.move(distance);
  X2.move(-distance);
}

void moveEast(int distance)
{ 
  X1.move(-distance);
  X2.move(distance);
}

void moveNorth(int distance)
{ 
  Y1.move(-distance);
  Y2.move(-distance);
}

void moveSouth(int distance)
{ 
  Y1.move(distance);
  Y2.move(distance);
}

void moveQuadrantI(int distance)
{
  X1.move(distance);
  X2.move(-distance);
  Y1.move(-distance);
  Y2.move(-distance);                                             
}

void moveQuadrantII(int distance)                                 //The rest of the movement commands are self explanatory
{
  X1.move(-distance);
  X2.move(distance);
  Y1.move(-distance);
  Y2.move(-distance);                                            
}

void moveQuadrantIII(int distance)
{
  X1.move(-distance);
  X2.move(distance);
  Y1.move(distance);
  Y2.move(distance);
}

void moveQuadrantIV(int distance)
{
  X1.move(distance);
  X2.move(-distance);
  Y1.move(distance);
  Y2.move(distance);
}

void goThroughTunnel0()
{
  whichSync = 0;
  moveWest(getMM(150));
  
  delay(100);
  whichSync = 1;
  moveNorth(getMM(450));
  delay(100);
  whichSync = 2;
  spin(getMM(ANGLE_CONVER*45));
  delay(100);
  moveQuadrantII(getMM(500));
  delay(100);
  whichSync = 1;
  moveNorth(getMM(1000));
  
}
void goThroughTunnel1()
{
  whichSync = 0;
  moveEast(getMM(150));
  delay(100);
  whichSync = 1;
  moveNorth(getMM(400));
  delay(100);
  whichSync = 2;
  spin(getMM(-ANGLE_CONVER*45));
  delay(100);
  moveQuadrantI(getMM(500));
  delay(100);
  whichSync = 1;
  moveNorth(getMM(1000)); 
}

double getMM(int distance){
  //22400 because it takes 4480 counts per revolution and *10 because Derek said so
  //d = 70.29 mm
  return (distance*4480)/220.8225476208265667;
}

