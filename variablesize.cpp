#include "macros.h"

int main(int argc, char *argv[]) IS
  bool b;
  char ch;
  short int si;
  int n;
  long L;
  long long LL;
  float f;
  double d;
  long double ld;
  void * p;
  
  cout << "size of a bool = " << sizeof b << endl;
  cout << "size of a char = " << sizeof ch << endl;
  cout << "size of a short int = " << sizeof si << endl;
  cout << "size of an int = " << sizeof n << endl;
  cout << "size of a long = " << sizeof L << endl;
  cout << "size of a long long = " << sizeof LL << endl;
  cout << "size of a float = " << sizeof f << endl;
  cout << "size of a double = " << sizeof d << endl;
  cout << "size of a long double = " << sizeof ld << endl;
  cout << "size of a void * = " << sizeof (void *) << endl;
  cout << "size of a void *p = " << sizeof p << endl;



  return 0;
END
