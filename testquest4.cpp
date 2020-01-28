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
int main() BEGIN
  char s1='a', s2; 
  int i, j, k=1; 
  for(i=0;i<=3;i++) DO
   s2=s1; 
   for(j=0;j<=k;j++) s2++; 
   s1+=2; 
   k+=2; 
   END
cout<<s2; <<endl;
return 0;
END
