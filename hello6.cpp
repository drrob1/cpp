#include <cstring>
#include <cstdio>
#include <iostream>
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
#define ADROF &
#define ADR(x) & ## x
#define DEREF *
#define POINTERTO *
#define VAR &
#define REF &

typedef FILE* FileType;
using ::MyFileType = FILE*; 
//    using MyFileType = FILE*;    doesn't compile.  spmething about a 
//                                 nested-name-specifier

  int main() IS
    cout << " Hello World!" <<endl;

//  int main() BEGIN
    printf(" Hello World! \n");
    return 0;
  END;
