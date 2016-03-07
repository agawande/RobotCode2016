
#include <AccelStepper.h>
#include <Wire.h>

#define SLAVE_ADDRESS 0x03

byte dataReceived[] = {0, 0, 0, 0};                             //This array of Bytes is where the incoming data from the Pi will be stored.


int state;                                                      //This variable is used to keep track of the state of the Motors. When the Motors are in Standby Mode waiting for a command and while they
                                                                //are moving, state is written to 1. When the motors reach their destination and the distance to move is 0, state is written to 0,
                                                                //for a very brief amount of time, (1 milisecond) and in this time, the Pi picks up that the command is completed. Once the Pi sends the 
                                                                //command to the Teensy 3.1, the Pi will wait in a while(1) loop, repeatedly checking the i2c bus to see if the Teensy has completed its operation.
                                                                //once the state is 0 for 1 ms, the Pi picks this up, and then sends a completed response to the Main Python program, through shared memory.


int numberOfByteReceived = 0;

boolean commandFinished = false;
boolean beenReached = false;

AccelStepper stepperX(1,14,15); // AccelStepper::DRIVER (3 pins) on (Driver Setting is (1), Step pin, Direction pin)
AccelStepper stepperY(1,16,17); // AccelStepper::DRIVER (3 pins) on (Driver Setting is (1), Step pin, Direction pin)
AccelStepper stepperZ(1,11,12); // AccelStepper::DRIVER (3 pins) on (Driver Setting is (1), Step pin, Direction pin)

void setup()
{  
  stepperX.setMinPulseWidth(1);
  stepperY.setMinPulseWidth(1);
  stepperZ.setMinPulseWidth(1);
  motorSetup();

  Serial.begin(9600);          //  setup serial
  
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  
  
  Wire.begin(SLAVE_ADDRESS);                                     //Start the I2C Bus as Slave on address
  Wire.onReceive(receiveEvent);                                  //Attach a function to trigger when something is received.
  Wire.onRequest(sendData); 
  //stepperZ.moveTo(1000);
}

void loop()
{
  //digitalWriteFast(22, HIGH);
    //if (stepperZ.distanceToGo() == 0)
     // stepperZ.moveTo(-stepperZ.currentPosition());
//  if (stop == true)
//  {
//  deposit1();
//  deposit2();
//  deposit3();
//  deposit4();
//  deposit5();
//  deposit6();
//  deposit7();
//  deposit8();
//  homePos();
//  }
//  
//  stop = false;
      
//      if (stepperX.distanceToGo() == 0 && stepperY.distanceToGo() == 0)
//      {
//      
//        if (stop == true)
//        {
//          delay(1000);
//          stepperX.moveTo(0);
//          stepperY.moveTo(0);
//          stop = false;
//        }
//        
//        else if (stop == false && stepperX.distanceToGo() == 0 && stepperY.distanceToGo() == 0)
//        {
//          digitalWriteFast(20, LOW);
//          digitalWriteFast(21, LOW);
//        }
//      }
//
//    stepperX.run();
//    //make State 1
  state = 1;
  
  //Whenever the distance to go reaches zero for both motors
  //(whenever a previous move had been called, the Accelstepper
  //library keeps track of the distance to go)
  //then send out a transmission to the Master signaling that
  //the current move function is complete
  if ((stepperX.distanceToGo() == 0 && stepperY.distanceToGo() == 0 && stepperZ.distanceToGo() == 0) && beenReached == true)
  {
    Serial.println("Completed command");
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
  //Return back to state 1
  state = 1;
  //The runAll function is repeatedly ran, this function branches out to the rest of the functions
   runAll();  
}

void motorSetup(void)
{
  //Step settings for X Motor
  pinMode(8, OUTPUT); //M0
  pinMode(9, OUTPUT); //M1
  pinMode(10, OUTPUT); //M2
  //Sleep Pin for X
  pinMode(20, OUTPUT); 
  
  //Step settings for X Motor
  digitalWriteFast(8, LOW); //M0
  digitalWriteFast(9, LOW); //M1
  digitalWriteFast(11, HIGH); //M2
  //Sleep Pin for X
  digitalWriteFast(20, LOW);
 
  stepperX.setMaxSpeed(11000);
  stepperX.setAcceleration(11000);
  
  //Step settings for Y Motor
  pinMode(5, OUTPUT); //M0
  pinMode(6, OUTPUT); //M1
  pinMode(7, OUTPUT); //M2
  //Sleep Pin for Y
  pinMode(21, OUTPUT);
  
  //Step settings for Y Motor
  digitalWriteFast(5, HIGH); //M0
  digitalWriteFast(6, HIGH); //M1
  digitalWriteFast(7, LOW); //M2
  //Sleep Pin for Y
  digitalWriteFast(21, LOW);
    
  stepperY.setMaxSpeed(1000000);
  stepperY.setAcceleration(800000);
  
  //Step settings for Z Motor
  pinMode(2, OUTPUT); //M0
  pinMode(3, OUTPUT); //M1
  pinMode(4, OUTPUT); //M2
  //Sleep Pin for Z
  pinMode(22, OUTPUT);
  
  //Step settings for Z Motor
  digitalWriteFast(2, LOW); //M0
  digitalWriteFast(3, HIGH); //M1
  digitalWriteFast(4, LOW); //M2
  //Sleep Pin for Z
  digitalWriteFast(22, LOW);
    
  stepperZ.setMaxSpeed(8000);
  stepperZ.setAcceleration(10000); 
}

void runAll()                                                    //The run all functions contains all of the commands to move the motors, update the PD control, and get a new encoder position for each motor
{
  stepperX.run();
  stepperY.run();                                                     
  stepperZ.run();
}

////Whenever the Teensy receieves a signal from the Master, this is ran.
////The function is immediately called and basically reads the four bytes
////that were sent form the master. 
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
        //Serial.print(X1.getGAbsolute());
        Serial.println("  ");
        Serial.print(" | Current Y Coordinate ");
        //Serial.print(Y2.getGAbsolute());
        Serial.println("  ");
        Serial.println("  -----------------------------");
         
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
      //int dist = ((0x0F & dataReceived[1]) << 8) | ((0x0F & dataReceived[2]) << 4) | (0x0F & dataReceived[3]);
      
    
      //Extract the directions by the same process of using a mask on the first byte, and using an AND operation.
      byte directions = dataReceived[0] & 0x0F;
      
      Serial.println("Command Decoded:");
      Serial.print(directions, HEX);
      //Now use the direction extracted determine how the motors should move
      //Home Position: 0x00 -> xxxx0000 
      if (directions == 0x00)
      {
        Serial.print("  Move Home: "); 
        Serial.print(directions);
        Serial.println("");                                            
        homePos();                                                     
      } 
      //Deposit 1: 0x01 -> xxxx0001 
        else if (directions == 0x01)
        {
          Serial.print("  Move Drop off 1: "); 
          Serial.print(directions);
          Serial.println("");
          deposit1();
        }
       //Deposit 2: 0x02 -> xxxx0010
        else if (directions == 0x02)
        {
          Serial.print("  Move Drop off 2: ");  
          Serial.print(directions);
          Serial.println("");
          deposit2();    
        }
       //Deposit 3: 0x03 -> xxxx0011
        else if (directions == 0x03)
        {
          Serial.print("  Move Drop off 3: ");  
          Serial.print(directions);
          Serial.println("");
          deposit3();
        }
        //Deposit 4: 0x04 -> xxxx0100 
        else if (directions == 0x04)
        {
          Serial.print("  Move Drop off 4: ");  
          Serial.print(directions);
          Serial.println("");
          deposit4();
        }
        //Deposit 5: 0x05 -> xxxx0101 
        else if (directions == 0x05)
        {
          Serial.print("  Move Drop off 5: ");  
          Serial.println("");
          deposit5();          
        }
        //Deposit 6: 0x06 -> xxxx0110
        else if (directions == 0x06)
        {
          Serial.print("  Move Drop off 6: ");  
          Serial.print(directions);
          Serial.println("");
          deposit6();
        }
        //Deposit 7: 0x07 -> xxxx0111
        else if (directions == 0x07)
        {
          Serial.print("  Move Drop off 7: ");  
          Serial.print(directions);
          Serial.println("");
          deposit7();
        }
        //Deposit 8: 0x08 -> xxxx1000
        else if (directions == 0x08)
        {
          Serial.print("  Move Drop off 8: ");  
          Serial.print(directions);
          Serial.println("");
          //deposit8();
          
           digitalWriteFast(22, HIGH);
           stepperZ.move(1000);
           while(true)
           {
             if (stepperZ.distanceToGo() == 0)
             {
                break;
             }
             runAll();
           }
            digitalWriteFast(22, LOW);
            beenReached = true;
        }
        //Deposit 5: 0x09 -> xxxx1001
        else if (directions == 0x09)
        {
          Serial.print("  Block Retrieval Point: ");  
          Serial.print(directions);
          Serial.println("");
          pickUpBlock();
        }
        else if (directions == 10)
        {
          Serial.print("  Sort position: ");  
          Serial.print(directions);
          Serial.println("");
          
        }
        else if (directions == 11)
        {
          Serial.print("  5 inch: ");  
          Serial.print(directions);
          Serial.println("");
          //stepperZ.moveTo(50);
        }
        else if (directions == 12)
        {
          Serial.print("  7 inch: ");  
          Serial.print(directions);
          Serial.println("");
          
        }
        else if (directions == 13)
        {
          Serial.print("  10 inch: ");  
          Serial.print(directions);
          Serial.println("");
          
        }
        else if (directions == 0x14)
        {
          Serial.print("  Coupling position: ");  
          Serial.print(directions);
          Serial.println("");
          
        }
        Serial.println(" ");
        Serial.println(" ????????????????????????????????????");
        Serial.println(" ? Waiting for completed command... ?");
        Serial.println(" ????????????????????????????????????");
        Serial.println(" ");
}
 

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
       Serial.print(stepperX.currentPosition());
       Serial.println(" ");
       Serial.print(" | Current Y Coordinate ");
       Serial.print(stepperY.currentPosition());
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

void homePos()
{
  stepperX.moveTo(0);
  stepperY.moveTo(0); 
  
  digitalWriteFast(20, HIGH);
  digitalWriteFast(21, HIGH);
  
  while(true)
  {
    if (stepperX.distanceToGo() == 0 && stepperY.distanceToGo() == 0)
    {
       break; 
    }
    runAll();     
  }  
  
  digitalWriteFast(20, LOW);
  digitalWriteFast(21, LOW);
  beenReached = true;
  //delay(200);
}

void deposit1()
{

      
  stepperX.moveTo(-25);
  stepperY.moveTo(14500);
  
   digitalWriteFast(20, HIGH);
   digitalWriteFast(21, HIGH);
  
  while(true)
  {
    if (stepperX.distanceToGo() == 0 && stepperY.distanceToGo() == 0)
    {
       break; 
    }
    runAll();     
  } 
  
  digitalWriteFast(20, LOW);
  digitalWriteFast(21, LOW); 
  beenReached = true; 
  //delay(200);
}

void deposit2()
{
  pickUpBlock();
      
  stepperX.moveTo(-1000);
  stepperY.moveTo(6000);
  
  digitalWriteFast(20, HIGH);
  digitalWriteFast(21, HIGH);
  
  while(true)
  {
    if (stepperX.distanceToGo() == 0 && stepperY.distanceToGo() == 0)
    {
       break; 
    }
    runAll();     
  } 
  
  digitalWriteFast(20, LOW);
  digitalWriteFast(21, LOW);
  beenReached = true;
  //delay(200);
}

void deposit3()
{
  pickUpBlock();
      
  stepperX.moveTo(-1000);
  stepperY.moveTo(12000);
  
  digitalWriteFast(20, HIGH);
  digitalWriteFast(21, HIGH);
  
  while(true)
  {
    if (stepperX.distanceToGo() == 0 && stepperY.distanceToGo() == 0)
    {
       break; 
    }
    runAll();     
  } 
  
  digitalWriteFast(20, LOW);
  digitalWriteFast(21, LOW);
  beenReached = true; 
  //delay(200);
}

void deposit4()
{
  pickUpBlock();
      
  stepperX.moveTo(-1000);
  stepperY.moveTo(18000);
  
  digitalWriteFast(20, HIGH);
  digitalWriteFast(21, HIGH);
  
  while(true)
  {
    if (stepperX.distanceToGo() == 0 && stepperY.distanceToGo() == 0)
    {
       break; 
    }
    runAll();     
  } 
  
  digitalWriteFast(20, LOW);
  digitalWriteFast(21, LOW);
  beenReached = true;
  //delay(200);
}

void deposit5()
{
  stepperX.moveTo(-1125);
  stepperY.moveTo(16000);
  
  digitalWriteFast(20, HIGH);
  digitalWriteFast(21, HIGH);  
  
  while(true)
  {
    if (stepperX.distanceToGo() == 0 && stepperY.distanceToGo() == 0)
    {
       break; 
    }
    runAll();     
  } 
  digitalWriteFast(20, LOW);
  digitalWriteFast(21, LOW);
  beenReached = true; 
  //delay(200);
}

void deposit6()
{
   pickUpBlock();
      
  stepperX.moveTo(-500);
  stepperY.moveTo(6000);

  digitalWriteFast(20, HIGH);
  digitalWriteFast(21, HIGH);
  
  while(true)
  {
    if (stepperX.distanceToGo() == 0 && stepperY.distanceToGo() == 0)
    {
       break; 
    }
    runAll();     
  } 
  digitalWriteFast(20, LOW);
  digitalWriteFast(21, LOW);
  beenReached = true;
  //delay(200);
}

void deposit7()
{
  pickUpBlock();
      
  stepperX.moveTo(-500);
  stepperY.moveTo(12000);

  digitalWriteFast(20, HIGH);
  digitalWriteFast(21, HIGH);
  
  while(true)
  {
    if (stepperX.distanceToGo() == 0 && stepperY.distanceToGo() == 0)
    {
       break; 
    }
    runAll();     
  } 
  
  digitalWriteFast(20, LOW);
  digitalWriteFast(21, LOW);
  beenReached = true;
  //delay(200); 
}

void deposit8()
{
  pickUpBlock();
      
  stepperX.moveTo(-500);
  stepperY.moveTo(18000);
 
  digitalWriteFast(20, HIGH);
  digitalWriteFast(21, HIGH);
  
  while(true)
  {
    if (stepperX.distanceToGo() == 0 && stepperY.distanceToGo() == 0)
    {
       break; 
    }
    runAll();     
  } 
  
  digitalWriteFast(20, LOW);
  digitalWriteFast(21, LOW);
  beenReached = true;
  //delay(200);
}

void pickUpBlock()
{
  stepperX.moveTo(-1149);
  stepperY.moveTo(26560);
  
  digitalWriteFast(20, HIGH);
  digitalWriteFast(21, HIGH);
  
  while(true)
  {
    if (stepperX.distanceToGo() == 0 && stepperY.distanceToGo() == 0)
    {
       break; 
    }
    runAll();     
  }  
  digitalWriteFast(20, LOW);
  digitalWriteFast(21, LOW);
  beenReached = true;
  //delay(200);
}

