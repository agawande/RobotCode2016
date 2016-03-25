
#include <iostream>
#include "messageformatter.hpp"

using namespace std;

/************************************************************
 * buildHeader()                                            *
 * The purpose of the header is to ensure data integrity    *
 * During i2c communication, individual bytes are sent one  *
 * at a time in sequential order. If the receiver is busy   *
 * when a byte is sent, it might miss some information.     *
 * The header has a key and a function code to allow the    *
 * receiver to determine whether it received everything or  *
 * not.                                                     *
 *                                                          *
 * A unique key is generated for each message and this key  *
 * is then added to the header of each byte.                *
 *                                                          *
 * The KEY cycles from MIN to MAX in increments of INCREMENT*
 * Settings of MIN = 0, MAX = 3, INCREMENT = 1 gives us     *
 * 4 unique keys to cycle through                           *
 *                                                          *
 * The the function code is a unique sequential value which *
 * is inserted into the header of each byte.                *
 * integrity.                                               *
 ***********************************************************/
unsigned int MessageFormatter::buildHeader()
{
   unsigned int header = 0;
   unsigned int tmpHeader = 0;
   unsigned int functionCode = 0;
   unsigned int functionCodeIncrementAmount = 4;

   // Used '<=' rather than '<' to give us one extra cycle without using addition
   for(int i = 0; i <= INT_BYTES; i++)
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

} //buildHeader

unsigned int MessageFormatter::buildCommand(unsigned int command)
{
   // Shift given command into the correct byte position and return it
   return command << (SHIFT_BYTE * BYTE_COMMAND);

} //buildCommand

unsigned int MessageFormatter::buildData(unsigned int data)
{
   unsigned int tmpData = 0;
   unsigned int dataBytes = 0;
   unsigned int dataMask = 0xf00; // 0b00000000_00000000_00001111_00000000

   for (int i = 0; i < DATA_NIBBLES; i++)
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
} //buildData


/************************************************************
 * buildMessage(command, data)                              *
 * A wrapper function which given a command and data        *
 * performs all tasks needed to build a message.            *
 ***********************************************************/
unsigned int MessageFormatter::buildMessage(unsigned int command, unsigned int data)
{
   unsigned int headerBytes = 0;
   unsigned int commandBytes = 0;
   unsigned int dataBytes = 0;
   headerBytes = buildHeader();
   commandBytes = buildCommand(command);
   dataBytes = buildData(data);

   updateKey();

   cout << "header " << headerBytes << endl;
   cout << "command " << commandBytes << endl;
   cout << "data " << dataBytes << endl;

   return headerBytes | commandBytes | dataBytes;
} //buildMessage

/************************************************************
 * buildMessage(command, data)                              *
 * A wrapper function which given a command and data        *
 * performs all tasks needed to build a message.            *
 ***********************************************************/
unsigned int MessageFormatter::buildMessageRaw(unsigned int command, unsigned int data1, unsigned int data2, unsigned int data3)
{
   unsigned int headerBytes = 0;
   unsigned int commandBytes = 0;
   unsigned int dataBytes = 0;
   headerBytes = buildHeader();
   commandBytes = buildCommand(command);

   dataBytes |= (data1 << 16);
   dataBytes |= (data2 << 8);
   dataBytes |= (data3);
   bitset<32> x(dataBytes);

   cout << "header " << headerBytes << endl;
   cout << "command " << commandBytes << endl;
   cout << "data " << x << endl;

   return headerBytes | commandBytes | dataBytes;
}


/************************************************************
 * checkData(data)                                          *
 * Simple data integrity check. Just makes sure that the    *
 * data value, after all the manipulations, does not exceed *
 * the maximum allowed value.                               *
 ***********************************************************/
unsigned int MessageFormatter::checkData(unsigned int data)
{
  if (data <= DATA_CEILING){
    return GOOD;
  } else {
    return BAD;
  }
} //checkData


/************************************************************
 * updateKey()                                              *
 * The key will be binary OR'd with other values to build   *
 * each I2C messages. The key is used to ensure data        *
 * integrity.                                               *
 *                                                          *
 * The KEY cycles from MIN to MAX in increments of INCREMENT*
 * Settings of MIN = 0, MAX = 3, INCREMENT = 1 gives us     *
 * 4 unique keys to cycle through                           *
 ***********************************************************/
void MessageFormatter::updateKey()
{
   if(key < KEY_MAX){
      key++;
   } else{
      key = KEY_MIN;
   }
} //updateKey
