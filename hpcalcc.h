// (C) 1990.  Robert W Solomon.  All rights reserved.
// This module simulates an RPN calculator, but with an 8 reg stack instead of 4.  All operations occur at the bottom of the stack

  enum StackRegNames {X,Y,Z,T5,T4,T3,T2,T1};
  const int StackSize = T1 + 1;

#include "macros.h"

RETURN string FUNCTION CropNStr(string instr);
RETURN string FUNCTION AddCommas(string instr);
PROCEDURE OutputFixedOrFloat(double r);
PROCEDURE PUSHX(double R);
RETURN double FUNCTION READX();
PROCEDURE GETSTACK(double STK[]);
PROCEDURE DumpStackFloat();
PROCEDURE DumpStackFixed();
PROCEDURE DumpStackGeneral();
PROCEDURE PushStacks();
PROCEDURE RollDownStacks();
PROCEDURE RollUpStacks();
RETURN string FUNCTION ToHex(double L);
RETURN double FUNCTION GetResult(string s);

