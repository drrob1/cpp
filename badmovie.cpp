#include "macros.h"

// from C++ All In One For Dummies (c) 2014
// I consider this instructive regarding pointer stuff
//
int main()
{
  string HorribleMovie;
  string *ptrToString;
  HorribleMovie = "L.A. Confidential";
  ptrToString = &HorribleMovie;
//  for (unsigned i = 0; i < HorribleMovie.length(); i++)
  for (unsigned i = 0; i < ptrToString->length(); i++)
  {
      cout << (*ptrToString)[i] << " ";
  }
  cout << endl;
  return 0;
}

