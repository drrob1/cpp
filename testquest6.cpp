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
  int ARR[1000], i, N, SUM;
  cout <<" N is (1..100) : ";
  cin >> N;
  if (N<1 || N>100) return -1;
  N=N*10;
  for(i=0;i<N;i++) ARR[i]= i%10;
  SUM =0;
  for(i=0;i<N;i=i+5) SUM += ARR[i];
  cout << SUM << endl;
  return 0;
END
