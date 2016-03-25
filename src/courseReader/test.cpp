#include <iostream>
#include <fstream>

#include <string>


using namespace std;


int main()

{
  ifstream f("hello");
  string s;
  getline(f, s);
  getline(f, s); //here is 3rd line

  cout << s << endl;
  f.close();

  return 0;
}
