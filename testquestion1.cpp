#include <stdio.h>
#include <iostream>  // must compile this using g++ -o hello hello.cpp for it to link correctly.
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

int main {
  char ch;
  char c[]  = "CS101PROGRAMMINGCS101.1x2014";
  char *p = c;
  cout  << p + p[1] - p[0];
//
// printf(" %s",p+p[1]-p[0]);
  return 0;
}
