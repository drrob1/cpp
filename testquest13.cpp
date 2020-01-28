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
int a=4,b=5; 
if(a++<=b && a>=++b){ 
   if(a==b){ 
      cout<<"IITBombay"; 
   } 
   else { 
      cout<<"IITPowai"; 
   } 
} 
else { 
   if(a!=b){ 
      cout<<"IITB"; 
   } 
   else { 
      cout<<"IITMumbai"; 
   } 
} 

  cout <<endl;


  return 0;
END
