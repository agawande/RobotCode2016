#include "../communication/i2c_setup.hpp"
#include "../communication/message_formatter.hpp"
#include "sorter.hpp"

using namespace std;

int main()
{
  Sorter sorter;

  sorter.toHeightBargeB();
  //sorter.goToD8();
}
