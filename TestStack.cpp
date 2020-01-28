#include "macros.h"
#include "hpcalcc.h"

RETURN int FUNCTION main() IS

//  ARRAYOF double TestStack[StackSize];
  int i;
//  char ch;
  int input;

  FOR i=1; i <= 8; i++ DO
    PUSHX(i);   // relying on implicit conversion here
  ENDFOR;
  PushStacks();


  FOR i=11; i< (11+StackSize); i++ DO
    PUSHX(i);
  ENDFOR;
  PushStacks();

  FOR i=21; i< (21+StackSize); i++ DO
    PUSHX(i);
  ENDFOR;
  PushStacks();

  FOR i=31; i< (31+StackSize); i++ DO
    PUSHX(i);
  ENDFOR;
  PushStacks();

  FOR i=41; i< (41+StackSize); i++ DO
    PUSHX(i);
  ENDFOR;
  PushStacks();


  cout << " StackMatrix init " << endl;

  REPEAT
    cout << " 1 -- RollDownStacks;  2 -- RollUpStacks; 3 -- PushStacks" << endl;
    cin >> input;
    SWITCH input DO
    case 1:
      RollDownStacks();
      DumpStackFixed();
      break;
    case 2:
      RollUpStacks();
      DumpStackFixed();
      break;
    case 3:
      PushStacks();
      break;
    default:
      DumpStackFixed();
    ENDCASE;
  UNTIL (input EQ 0);


END; // main  

  

 
