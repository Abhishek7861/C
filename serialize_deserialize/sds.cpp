#include <iostream>
#include <string>

using namespace std;

struct msg_t {
    string a;
      string toString()
  {
    return a;
  }
};


struct movies_t {
  string title;
  int year;
  string inbox;

  string toString()
  {
    return "title = " + title + ", year = " + to_string(year) + " " + inbox;
  }
};

int main()
{
    struct msg_t b{"hello"};
    cout << b.toString()<< endl;
    struct movies_t m{"Odyssey", -800, b.toString()};
    cout << m.toString() << endl;
}