#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

class MessageFormatter {
public:
  MessageFormatter()
  {
    i,key = 0;
  }

private:
  unsigned int
  buildHeader()
  {
    unsigned int header = 0;
    unsigned int tmpHeader = 0;
    unsigned int functionCode = 0;
    unsigned int functionCodeIncrementAmount = 4;

    // Used '<=' rather than '<' to give us one extra cycle without using addition

    for(i = 0; i <= INT_BYTES; i++)
    {
      tmpHeader = 0;

      // Combine functionCode and key
      tmpHeader = functionCode | key;

      // Shift tmpHeader out of the data byte and into the header byte
      tmpHeader = tmpHeader << SHIFT_NIBBLE;

      // Copy tmpHeader into header
      header = header | tmpHeader;

      // If not least significant byte, shift header contents & update functionCode
      if( i < INT_BYTES - 1)
      {
        header = header << SHIFT_BYTE;

        functionCode += functionCodeIncrementAmount;
      }
    }

    return header;
   }

  unsigned int
  buildCommand(unsigned int command)
  {
    // Shift given command into the correct byte position and return it
    return command << (SHIFT_BYTE * BYTE_COMMAND);
  }

  unsigned int
  buildData(unsigned int data)
  {
    unsigned int tmpData = 0;
    unsigned int dataBytes = 0;
    unsigned int dataMask = 0xf00; // 0b00000000_00000000_00001111_00000000

    for (i = 0; i < DATA_NIBBLES; i++)
    {
      // Extract data and copy to tmpData
      tmpData = data & dataMask;

      // shift tmpData to least significant position
      tmpData = tmpData >> (SHIFT_NIBBLE * ((DATA_NIBBLES - 1) - i));

      // copy tmpData into dataBytes
      dataBytes = dataBytes | tmpData;

      //shift dataBytes by a byte depending on iteration number
      if (i < (DATA_NIBBLES -1))
      {
        dataBytes = dataBytes << SHIFT_BYTE;
      }

      // Shift dataMask by a nibble
      dataMask = dataMask >> SHIFT_NIBBLE;
    }

    return dataBytes;
  }

public:
  unsigned int
  buildMessage(unsigned int command, unsigned int data)
  {
    unsigned int headerBytes = 0;
    unsigned int commandBytes = 0;
    unsigned int dataBytes = 0;

    headerBytes = buildHeader();
    commandBytes = buildCommand(command);
    dataBytes = buildData(data);

    std::cout << "header " << headerBytes << endl;
    std::cout << "command " << commandBytes << endl;
    std::cout << "data " << dataBytes << endl;

    return headerBytes | commandBytes | dataBytes;
  }

  unsigned int
  checkData(unsigned int data)
  {
    if (data <= DATA_CEILING){
      return GOOD;
    } else {
      return BAD;
    }
  }

  void
  updateKey()
  {
    if(key < KEY_MAX){
      (key)++;
    } else{
      key = KEY_MIN;
    }
  }


private:
  int i;
  unsigned int key;
/************************************************************
 * The key will be binary OR'd with other values to build   *
 * each I2C messages. The key is used to ensure data        *
 * integrity.                                               *
 *                                                          *
 * The KEY cycles from MIN to MAX in increments of INCREMENT*
 * Settings of MIN = 0, MAX = 3, INCREMENT = 1 gives us     *
 * 4 unique keys to cycle through                           *
 ***********************************************************/
};
