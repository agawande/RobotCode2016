#include <AccelStepper.h>
#include <Wire.h>

#define SLAVE_ADDRESS 0x05

byte dataReceived[] = {0, 0, 0, 0};                             //This array of Bytes is where the incoming data from the Pi will be stored.

int numberOfByteReceived = 0;

//SCL, pin 0
//SDA, pin 1

//Cage Conveyers
int C1D1 = 2;
int C1D2 = 3;
//pseudo PWM
int CC1 = 4;   //i.e. conveyer of cage

int C2D1 = 5;
int C2D2 = 6;
//pseudo PWM
int CC2 = 7;  //SPEED CONTROL

int C3D1 = 8;
int C3D2 = 9;
//pseudo PWM
int CC3 = 10;

int C4D1 = 11;
int C4D2 = 12;
//pseudo PWM
int CC4 = 13;

//Main Conveyer
int MainConvD1 = 23;
int MainConvD2 = 22;
//pseudo PWM
int MainConv = 21;

//Grabber Conveyer Motor
int GrabberDir1 = 20;
int GrabberDir2 = 19;
//pseudo PWM
int GrabberConv = 18;

//Remember to connect the STBY pin to a HIGH for the TB driver

//Grabber Clamper Stepper
int clamperDir = 17;
int clamperPin = A7;  //Using analog pin - no digital pin left

AccelStepper clamper(1, clamperPin, clamperDir);  // AccelStepper::DRIVER (3 pins) on (Driver Setting is (1), Step pin, Direction pin)

//Coupling Motors
int Coup1 =  14;  //PWM
int Coup1Dir = A3;
int senseCoup1 = A0;  //To sense current

int Coup2 = 15;
int Coup2Dir = A2;
int senseCoup2 = A1;

void setup()
{
  clamper.setMinPulseWidth(1);
  clamper.setMaxSpeed(11000);
  clamper.setAcceleration(11000);
  
  Serial.begin(9600);          //  setup serial
  
  Wire.begin(SLAVE_ADDRESS);                                     //Start the I2C Bus as Slave on address
  Wire.onReceive(receiveEvent);                                  //Attach a function to trigger when something is received.
  Wire.onRequest(sendData);

  pinMode(C1D1, OUTPUT);
  pinMode(C1D2, OUTPUT);
  pinMode(C2D1, OUTPUT);
  pinMode(C2D2, OUTPUT);
  pinMode(C3D1, OUTPUT);
  pinMode(C3D2, OUTPUT);
  pinMode(C4D1, OUTPUT);
  pinMode(C4D2, OUTPUT);
  pinMode(MainConvD1, OUTPUT);
  pinMode(MainConvD2, OUTPUT);
  pinMode(GrabberDir1, OUTPUT);
  pinMode(GrabberDir2, OUTPUT);
  pinMode(clamperDir, OUTPUT);
  pinMode(clamperPin, OUTPUT);

  pinMode(Coup1. OUTPUT);
  pinMode(Coup1Dir, OUTPUT);
  pinMode(senseCoup1, INPUT);

  pinMode(Coup2. OUTPUT);
  pinMode(Coup2Dir, OUTPUT);
  pinMode(senseCoup2, INPUT);

  pinMode(CC1, OUTPUT);
  pinMode(CC2, OUTPUT);
  pinMode(CC3, OUTPUT);
  pinMode(CC4, OUTPUT);
  pinMode(MainConv, OUTPUT);
  pinMode(GrabberConv, OUTPUT);

  //Might need to change these according to direction
  digitalWrite(C1D1, HIGH);
  digitalWrite(C1D2, LOW);
  digitalWrite(C2D1, HIGH);
  digitalWrite(C2D2, LOW);
  digitalWrite(C3D1, HIGH);
  digitalWrite(C4D2, LOW);
  digitalWrite(MainConvD1, HIGH);
  digitalWrite(MainConvD2, LOW);
  digitalWrite(GrabberDir1, HIGH);
  digitalWrite(GrabberDir2, LOW);
}

void loop()
{
  //sleep for 1ms to relenquish the processor
  delay(1);
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
      
      //Now use the direction extracted determine how the motors should move
      if (directions == 0x00)
      {
        Serial.print("  Cage Conveyer 1 On: ");                                       
        switchMotion(CC1);   //switch the state, if on then off, if off then on
        digitalWrite(CC1, HIGH);
      }
      else if (directions == 0x01)
      {
        Serial.print("  Cage Conveyer 1 Off: ");
        digitalWrite(CC1, LOW);
      }
      else if (directions == 0x02)
      {
        Serial.print("  Cage Conveyer 2 On: ");
        digitalWrite(CC2, HIGH);
      }
      else if (directions == 0x03)
      {
        Serial.print("  Cage Conveyer 2 Off: ");
        switchMotion(CC2, LOW);
      }
      else if (directions == 0x04)
      {
        Serial.print("  Cage Conveyer 3 On: ");
        switchMotion(CC3, HIGH);
      }
      else if (directions == 0x05)
      {
        Serial.print("  Cage Conveyer 3 Off: ");
        switchMotion(CC3, LOW);
      }
      else if (directions == 0x06)
      {
        Serial.print("  Cage Conveyer 4 On: ");
        switchMotion(CC4, HIGH);
      }
      else if (directions == 0x07)
      {
        Serial.print("  Cage Conveyer 4 Off: ");
        switchMotion(CC4, LOW);
      }
      else if (directions == 0x08)
      {
        Serial.print("  Main Conveyer: ");
        switchMotion(MainConv);
      }
      else if (directions == 0x09)
      {
        Serial.print("  Grabber Conveyer On: ");  
        digitalWrite(GrabberConv, HIGH);
      }
      else if (directions == 0x10)
      {
        Serial.print("  Grabber Conveyer Off: ");
        digitalWrite(GrabberConv, ON);
      }
      else if (directions == 0x11)
      {
        Serial.print("  Clamper Close: ");
        //Test and change
        clamper.moveTo(90);
      }
      else if (directions == 0x12)
      {
        Serial.print("  Clamper Open: ");
        //Test and change
        clamper.moveTo(-90);
      }
      else if (directions == 0x13)
      {
        Serial.print("  Couple ");
        Serial.println("");
        Couple();
      }
      else if (directions == 0x14)
      {
        Serial.print(" DeCouple ");  
        Serial.print(directions);
        Serial.println("");
        DeCouple();
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

void Couple()
{  
  //Direction, might need to reverse it
  digitalWrite(Coup1Dir, HIGH);
  digitalWrite(Coup2Dir, LOW);

  controlCouplingMotors(); 
}

void DeCouple()
{
  digitalWrite(Coup1Dir, LOW);
  digitalWrite(Coup2Dir, HIGH);

  controlCouplingMotors();
}

void controlCouplingMotors()
{
  analogWrite(Coup1, 255);
  analogWrite(Coup2, 255);
  
  //Prelimnary code for coupler
  while(1){
    int sensorValue1 = analogRead(senseCoup1);
    int sensorValue2 = analogRead(senseCoup2);
    //Randomly just 1
    if(sensorValue1 > 1 && sensorValue2 > 1){
       analogWrite(Coup1, 0);
       analogWrite(Coup2, 0);
    }
  }
}

