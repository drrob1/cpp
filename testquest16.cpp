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
int i, countIf=0, countElse=0; 
for(i=1;i<5;i++) 
   if((i * i * i + 2) > (i * i + 3))  
      countIf++; 
   else 
      countElse++; 
cout<<countIf<<","<<countElse;

cout<<endl;

  return 0;
END
