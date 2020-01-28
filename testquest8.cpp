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
  int i, j=0, n=8, y=12; 
  int A[8]={5,7,12,15,20,2,0,30}; 
  for(i=0;i<n;i++) DO
    if(A[i] < y) THEN
      int tmp = A[j]; 
      A[j] = A[i]; 
      A[i] = tmp; 
      j++; 
    ENDIF
  ENDFOR
  int s=0; 
  for(i=0;i<n;i++)   s=s+A[i];  
cout<<s/n; 

cout <<endl;
cout <<endl;

  return 0;
END
