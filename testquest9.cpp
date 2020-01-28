#include <stdio.h>
#include <iostream>
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
using namespace std;  // a compiler directive that prefixes each variable name, ie, std::myvariablename.  Since I rarely see it, I'm guessing it is optional.

int f(int n) IS
    int r = 0;
    if(n <= 0) return 1;
    else if(n > 3) THEN
        r = n;
        return f(n-2) + 2;
    ENDIF;
    return f(n-1) + r;
END;

int main() BEGIN
  int N=5, q;
  q=  f(N);

  cout <<" f(5) is " <<q <<endl;



  return 0;
END
