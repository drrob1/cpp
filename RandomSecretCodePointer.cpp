//RandomSecretCodePointer
//#include <iostream>
//#include <sstream>
//#include <stdlib.h>
//using namespace std;
//
#include "macros.h"

RETURN string PtrType FUNCTION GetSecretCode()
{
  string PtrType code = new string;
  code->append("CR");
  int randomnumber = rand();
  ostringstream converter;
  converter << randomnumber;
  code->append(converter.str());
  code->append("NQ");
  return code;
}
/*
int main()
{
  string PtrType newcode;                     // newcode is declared as a string pointer type.
  int index;
  for (index = 0; index < 10; index++)
  {
    newcode = GetSecretCode();
    cout << *newcode << endl;
  }
  return 0;
}
*/

int main()
{
  string newcode;                     // newcode is NOT declared as a string pointer type.
  int index;
  for (index = 0; index < 10; index++)
  {
    newcode = DEREF GetSecretCode();
    cout << newcode << endl;
  }
  return 0;
}
