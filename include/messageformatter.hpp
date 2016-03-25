#ifndef MESSAGEFORMATTER_H
#define MESSAGEFORMATTER_H

#include <bitset>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

// Constants for shifting a variable's data
#define SHIFT_2BITS     2
#define SHIFT_NIBBLE    4
#define SHIFT_BYTE      8
#define INT_BYTES       4

// Byte position contents ( MSB 3-2-1-0 LSB)
#define BYTE_COMMAND    3
#define BYTE_DATA_3     2
#define BYTE_DATA_2     1
#define BYTE_DATA_1     0

// Key constants
#define KEY_MIN         0
#define KEY_MAX         3

// Data constants
#define DATA_NIBBLES    3
#define DATA_CEILING    0xfff // 2^((DATA_NIBBLES*4)-1)
#define GOOD            1
#define BAD            -1

using namespace std;

class MessageFormatter {
   public: 
     
      //Constructor
      MessageFormatter() { key = 0; }

      //Getters & Setters
      //unsigned int getKey();
      //void setKey(unsigned int myKey);

      //Public methods
      unsigned int buildMessage(unsigned int command, unsigned int data);
      unsigned int buildMessageRaw(unsigned int command, unsigned int data1, unsigned int data2, unsigned int data3);
      unsigned int checkData(unsigned int data);
      void updateKey();

   private: 
     
      //Private methods
      unsigned int buildHeader();
      unsigned int buildCommand(unsigned int command);
      unsigned int buildData(unsigned int data);

      //Private data members
      //int i;
      unsigned int key;

};

#endif
