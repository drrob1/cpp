#include "macros.h"

int main()
{
   float number;
 
   number = 480.0;
   number /= 100.0;
   number *= 100.0;
//   480.0 / 100.0 * 100.0;  This expression is evaluated as a double before assigned to
//   float.  At least according to a poster on the list.  Splitting it up stops that.
//   cout << " Result = " << number << endl;
   printf("result: %3.15f\n", number);
//
 
   return 0;
}
