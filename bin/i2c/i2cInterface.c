#include <unistd.h>
#include "i2cInterface.h"

int main ()
{
   int i = 0;



   unsigned int header         = 0;
   unsigned int commandBytes = 0;
   unsigned int dataBytes      = 0;
   unsigned int message        = 0;

   unsigned int key = KEY_MIN;
   unsigned int command = PIVOT_RIGHT;
   unsigned int data = 0xfff;

   //Let's just loop around for testing purposes
   while ( i < 5 )
   {
      printf("------------------------------------\n");

      if (checkData(data) == GOOD)
      {   
         //printf("Data is good!\n");
         //printf("Data           = %016x\n", data);

         //printf("Key Before     = %016x\n", key);
      
         //header = buildHeader(key);
         //printf("Header         = %016x\n", header);

         //commandBytes = buildCommand(command);
         //printf("CommandBytes = %016x\n", commandBytes);

         //dataBytes = buildData(data);
         //printf("DataBytes      = %016x\n", dataBytes);

         message = buildMessage(key, command, data);
         printf("Message        = %016x\n", message);

         updateKey(&key);
         //printf("Key After      = %016x\n", key);
      }
      else
      {
         printf("Data is no good!\n");
      }
      
      sleep(1);

      i++;
   }

   return 1;
}
