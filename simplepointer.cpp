#include <stdio.h>
#include <iostream>
using namespace std;

#define EQ ==
#define NE !=
#define BEGIN {
#define END }
#define ENDIF }
#define ENDFOR }
#define ENDCASE }
#define ENDLOOP }
#define MOD %
#define AND &&
#define OR ||
#define NOT !
#define THEN {
#define ELSE }else{
#define ELSIF }else if{
#define IS {
#define DO {
#define LOOP {
// using namespace std;  // a compiler directive that prefixes each variable name, ie, std::myvariablename.  Since I rarely see it, I'm guessing it is optional.

int main() IS
  int a;
  float b;
  char c = 'c';

  int *ptrA;
  float *ptrB;
  char *ptrC;

  ptrA = &a; cout << " Address of a is: " << ptrA <<endl;
  ptrB = &b; cout << " Address of b is: " << ptrB <<endl;
  ptrC = &c; cout << " Address of c is: " << ptrC <<endl;



  printf(" Hello World! \n");
  return 0;
END
