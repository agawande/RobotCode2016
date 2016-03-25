#ifndef I2CDISPATCHER_H
#define I2CDISPATCHER_H

using namespace std;

static const char *devName = "/dev/i2c-1";

class I2cDispatcher
{
   public: 

     //Constructors
     I2cDispatcher(int i2cAddress) : i2cAddress(i2cAddress) { setupConnection(); }

     //Public methods
     int  getFile();
     void setupConnection();
     void sendData(unsigned int data); //This is the function to send the command to the Teensy via i2c

   private:

      const int i2cAddress;
      int file;
};

#endif
