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
  int N=5, i, j, a[11], b[11]; 
  a[0]=3; 
  b[0]=0; 
  a[1]=2; 
  for(i=1;i<N;i=i+1) DO
    b[i]=a[i-1]+b[i-1]; 
    if(i>=2) a[i]=a[i-1]+b[i-2]; 
  ENDFOR 
  i=0; 
  while(i<N) DO
    a[2*(N-i-1)]=a[N-i-1]; 
    a[2*(N-i-1)+1]=b[N-i-1]; 
    i++; 
  ENDLOOP 
  for(i=0;i<2*N;i++)    cout << a[i]<<" "; 

cout <<endl;
cout <<endl;
cout <<endl;
END
