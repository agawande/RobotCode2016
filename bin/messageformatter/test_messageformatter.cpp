#include <iostream>
#include "messageformatter.hpp"

using namespace std;

int main()
{
   unsigned int command = 3;
   unsigned int data = 1000;

   MessageFormatter messageFormatter;

   cout << "Message: " << messageFormatter.buildMessage(command, data) << endl;;

   return 0;
}

