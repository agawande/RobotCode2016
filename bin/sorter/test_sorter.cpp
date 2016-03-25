#include "../communication/i2c_setup.hpp"
#include "../communication/message_formatter.hpp"
#include "sorter.hpp"

using namespace std;

int main()
{
  Sorter sorter;

  //sorter.sort(RED, BIG, 0);

  sorter.sort(BIG, YELLOW, 1);
}
