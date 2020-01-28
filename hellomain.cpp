//#pragma once  default is on
#include <cstring>
#include <cstdio>
#include <iostream>
using namespace std; 
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <ctime>

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
#define ADROF &
#define ADR &
#define DEREF *
#define POINTERTO *
#define VAR &
#define REF &
#define PROCEDURE void
#define FUNCTION

typedef FILE* FileType;
// using MyFileType = FILE*;

PROCEDURE hello();  // This did not compile when I left out the ()

int main() BEGIN
  hello();          // This did not compile when I left out the ()
  cout << " World" <<endl;



  return 0;
END
