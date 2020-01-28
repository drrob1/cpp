#include "macros.h"
#include "hpcalcc.h"
#include "tokenizec.h"
#include "getcommandline.h"
#include "timlibc.h"

  double Stack[T1];
  const double PI = 3.141592653589793;
  double LastX=0.0;
  double MemReg=0.0;


//------------------------------------------------------
PROCEDURE STACKUP() IS
  int S;
  FOR S = T2; S >= X; --S DO
    Stack[S+1] = Stack[S];
  ENDFOR;
ENDFUNC;// STACKUP
//------------------------------------------------------
//PROCEDURE STACKDN() IS
//  int S;
//  FOR S = Z; S <= T1; ++S DO 
//    Stack[S-1] = Stack[S];
//  ENDFOR;
//ENDFUNCT; // STACKDN
//------------------------------------------------------
