#include <PDEncoderMotor.h>
#include <Encoder.h>

  int PD_FREQUENCY = 1000;
  
  Encoder X1Encoder(0, 1);
  Encoder X2Encoder(2, 3);
  Encoder Y1Encoder(6, 7);
  Encoder Y2Encoder(4, 5);
  
  long positionY1 = -999;
  long positionY2 = -999;
  long positionX1 = -999;
  long positionX2 = -999;
  
  long newPosY1, newPosY2, newPosX1, newPosX2;
  
  int difference = 0;
  int variableSpeed = 0;
  
  IntervalTimer X1Timer;
  IntervalTimer X2Timer;
  IntervalTimer Y1Timer;
  IntervalTimer Y2Timer;
  
  PDEncoderMotor X1(16,15,23); 
  PDEncoderMotor X2(14,13,22); 
  PDEncoderMotor Y1(11,12,20); 
  PDEncoderMotor Y2(9,10,21); 
  
  float kp = 20;
  float kd = .2;
  float stepSize = 35;
  float pm = 0;
  float pr = 0;
  float vm = 0;
  
  int counter = 0;
  
  int moveDistanceX = 0;
  int moveDistanceY = 0;
  
void setup()
{
  X1.setVariables(kp, kd, stepSize, pm, pr, vm);
  X2.setVariables(kp, kd, stepSize, pm, pr, vm);
  Y1.setVariables(kp, kd, stepSize, pm, pr, vm);
  Y2.setVariables(kp, kd, stepSize, pm, pr, vm);
  
  X1.invertPins();
  Y2.invertPins();
  
  X1.initializeMotor();
  X2.initializeMotor();
  Y1.initializeMotor();
  Y2.initializeMotor();
  
  
  X1Timer.begin(interruptX1, 1000);
  X2Timer.begin(interruptX2, 1000);
  Y1Timer.begin(interruptY1, 1000);
  Y2Timer.begin(interruptY2, 1000); 
  
  //X1.moveTo(100000);
  //X2.moveTo(100000);
  
  moveDistanceY = 50000;
  
  Y1.moveTo(moveDistanceY);
  Y2.moveTo(moveDistanceY);
}

void loop()
{  
  
  runAll();
  if (((abs(newPosY1) == 10000) && (abs(newPosY2) == 10000)) && ((abs(newPosX1) == 0) && (abs(newPosX2) == 0)))  
  {
    counter++;
    
    if (counter == 10000)
    {
      moveDistanceX = 50000;
      delay(1000);
      X1.moveTo(moveDistanceX);
      X2.moveTo(moveDistanceX);
      counter = 0;
    }   
  }
  
  if (((abs(newPosY1) == 10000) && (abs(newPosY2) == 10000)) && ((abs(newPosX1) == 10000) && (abs(newPosX2) == 10000)))  
  {
    counter++;
    
    if (counter == 10000)
    {
      moveDistanceY = 0;
      delay(1000);
      Y1.moveTo(moveDistanceY);
      Y2.moveTo(moveDistanceY); 
      counter = 0;
    }      
  } 
  
  if (((abs(newPosY1) == 0) && (abs(newPosY2) == 0)) && ((abs(newPosX1) == 10000) && (abs(newPosX2) == 10000)))  
  {
        counter++;
    
    if (counter == 10000)
    {
      moveDistanceX = 0;
      delay(1000);
      X1.moveTo(moveDistanceX);
      X2.moveTo(moveDistanceX);   
      counter = 0;
    }  
  }
}


void runAll()
{
  X1.run();
  X1.setCurrentCount(X1Encoder.read());
  X2.run();
  X2.setCurrentCount(X2Encoder.read());
  Y1.run();
  Y1.setCurrentCount(Y1Encoder.read());
  Y2.run();
  Y2.setCurrentCount(Y2Encoder.read());
  
  newPosY1 = Y1Encoder.read();
  newPosY2 = Y2Encoder.read();
  newPosX1 = X1Encoder.read();
  newPosX2 = X2Encoder.read();
  
  if ((abs(newPosX1) != abs(newPosX2))  && ((newPosX1 != positionX1) && (newPosX2 != positionX2))) 
  {
    synchronizeX();
    
  }
  if ((abs(newPosY1) != abs(newPosY2)) && ((newPosY1 != positionY1) && (newPosY2 != positionY2))) 
  {
    synchronizeY();
  }
  
  //Serial.println(newPosY1);
  //Serial.println(newPosY2);
  //Serial.println(newPosX1);
  //Serial.println(newPosX2);
}

//void setDirectionX()
//{
//  if (newPosX2 == moveDistanceX)
//  {
//  
//  }
//    else if (moveDistanceX - newPosX2 > 0)
//    {
//      digitalWrite(16, HIGH);
//      digitalWrite(15, LOW);
//      
//      digitalWrite(14, LOW);
//      digitalWrite(13, HIGH);
//    }  
//    else if (moveDistanceX - newPosX2 < 0)
//    {
//      digitalWrite(16, LOW);
//      digitalWrite(15, HIGH);
//      
//      digitalWrite(14, HIGH);
//      digitalWrite(13, LOW);
//    }
//}
//
//void setDirectionY()
//{
//  if (newPosY1 == moveDistanceY)
//  {
//  
//  }
//    else if (moveDistanceY - newPosY1 > 0)
//    {
//      digitalWrite(11, LOW);
//      digitalWrite(12, HIGH);
//      
//      digitalWrite(9, HIGH);
//      digitalWrite(10, LOW);
//    }  
//    else if (moveDistanceX - newPosX2 < 0)
//    {
//      digitalWrite(11, HIGH);
//      digitalWrite(12, LOW);
//      
//      digitalWrite(9, LOW);
//      digitalWrite(10, HIGH);
//    }
//}


void synchronizeY()
{
    if (abs(newPosY2) > abs(newPosY1))
    {
      difference = abs(newPosY2) - abs(newPosY1);
      int tempSpeed = Y2.getSpeed();
  
      variableSpeed  = abs(tempSpeed) - (difference);
      variableSpeed  = map(variableSpeed , 0, 255, 36, 255);
      //setDirectionY();
      analogWrite(20, tempSpeed);//Sets speed variable via PWM 
      //digitalWrite(11, LOW);
      //digitalWrite(12, HIGH);
      
      analogWrite(21, variableSpeed);
      //digitalWrite(9, HIGH);
      //digitalWrite(10, LOW);
      
      positionY1 = newPosY1;
      positionY2 = newPosY2;
    }  
      
    if (abs(newPosY1) > abs(newPosY2))
    {
      difference = abs(newPosY1) - abs(newPosY2);
      int tempSpeed = Y1.getSpeed();
  
      variableSpeed = abs(tempSpeed) - (difference);
      variableSpeed  = map(variableSpeed , 0, 255, 36, 255);
      //setDirectionY();
      analogWrite(21, tempSpeed);
      //digitalWrite(9, HIGH);
      //digitalWrite(10, LOW);
       
      analogWrite(20, variableSpeed);//Sets speed variable via PWM 
      //digitalWrite(11, LOW);
      //digitalWrite(12, HIGH);
  
      positionY1 = newPosY1;
      positionY2 = newPosY2;
    }
}

void synchronizeX()
{
    if (abs(newPosX2) > abs(newPosX1))
    {
      difference = abs(newPosX2) - abs(newPosX1);
      int tempSpeed = X2.getSpeed();
      
      variableSpeed = abs(tempSpeed) - difference;
      variableSpeed  = map(variableSpeed , 0, 255, 36, 255);
      //setDirectionX();
      analogWrite(23, tempSpeed);//Sets speed variable via PWM 
      //digitalWrite(16, HIGH);
      //digitalWrite(15, LOW);
      
      analogWrite(22, variableSpeed);
      //digitalWrite(14, LOW);
      //digitalWrite(13, HIGH);
  
      positionX1 = newPosX1;
      positionX2 = newPosX2;
    }
    if (abs(newPosX1) > abs(newPosX2))
    {      
      difference = abs(newPosX1) - abs(newPosX2);
      int tempSpeed = X1.getSpeed();
      //setDirectionX();
      variableSpeed = abs(tempSpeed) - difference;
      variableSpeed  = map(variableSpeed , 0, 255, 36, 255);
      analogWrite(22, tempSpeed);
      //digitalWrite(14, LOW);
      //digitalWrite(13, HIGH);
       
      analogWrite(23, variableSpeed);//Sets speed variable via PWM 
      //digitalWrite(16, HIGH);
      //digitalWrite(15, LOW);
  
      positionX1 = newPosX1;
      positionX2 = newPosX2;  
    }
}
//    else if ((moveDistanceX - newPosX2) < 0)
//    { 
//      
//      if (abs(newPosX2) > abs(newPosX1))
//      {
//        difference = abs(newPosX2) - abs(newPosX1);
//        int tempSpeed = X2.getSpeed();
//        
//        variableSpeed = tempSpeed - difference;
//        analogWrite(23, variableSpeed);//Sets speed variable via PWM 
//        digitalWrite(16, LOW);
//        digitalWrite(15, HIGH);
//        
//        analogWrite(22, tempSpeed);
//        digitalWrite(14, HIGH);
//        digitalWrite(13, LOW);
//    
//        positionX1 = newPosX1;
//        positionX2 = newPosX2;
//      }
//      
//      if (abs(newPosX1) > abs(newPosX2))
//      {      
//        difference = abs(newPosX1) - abs(newPosX2);
//        int tempSpeed = X1.getSpeed();
//    
//        variableSpeed = tempSpeed - difference;
//        analogWrite(22, variableSpeed);
//        digitalWrite(14, HIGH);
//        digitalWrite(13, LOW);
//         
//        analogWrite(23, tempSpeed);//Sets speed variable via PWM 
//        digitalWrite(16, LOW);
//        digitalWrite(15, HIGH);
//    
//        positionX1 = newPosX1;
//        positionX2 = newPosX2;  
//      }
//    }

void interruptX1(void) 
{
  int temp = X1.getGMSticks();
  temp++;
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
