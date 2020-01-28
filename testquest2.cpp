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
  int i,j,sum=10; 
  for(i=0;i<5;i++) 
   if(i%2) 
      for(j=0;j<=3;sum+=j,j++){ } 
   else 
      for(j=3;j>0;sum+=j,j--){ } 
  cout<<sum <<endl;
END
