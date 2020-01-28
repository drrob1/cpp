#include "macros.h"
#include "hpcalcc.h"
#include "tokenizec.h"
#include "getcommandline.h"
#include "timlibc.h"
#include "holidaycalc.h"

/* (C) 1990.  Robert W Solomon.  All rights reserved.
  REVISION HISTORY
  ----------------
   1 Dec 89 -- Added the help command.
  24 Dec 91 -- Converted to M-2 V 4.00.  Also changed the params to the
                GETRESULT proc to be more reasonable.
  21 Mar 93 -- Added exponentiation and MOD, INT, FRAC and ROUND, as well as used the UL2 procs again.
  25 Jul 93 -- Added JUL and GREG commands.
  18 May 03 -- Win32 version using Stony Brook Modula-2 v 4
  26 May 03 -- Allowed years to pivot for 2000 or 1900 in TIMLIB.
   4 Oct 03 -- Added LongCard2HexStr and its cmd.
  31 Oct 03 -- Added HCF cmd.
   1 Nov 03 -- Fixed the var swap bug in HCF rtn.
  27 Nov 04 -- Added pi as a cmd.
  12 Mar 06 -- Made trig fcn's use arguments in degrees, changed stacksize to const, and moved stackregnames to def.
  22 Jul 06 -- Added % operator does xy/100 but does not drop stack.  Just like old HP 25.
  15 Apr 07 -- Added comma to push stack.  Removed it as a delim char and made it a ALLELSE.  And
                added ! ` and ~ as stack commands.  And updated i/o procedure use.
   1 Apr 13 -- Modified the DUMP routine so the characters are not printed on the same line as the numbers.  The
                alignment was all wrong anyway.  And added Dump2Console.
   2 May 13 -- Will use consolemode compilation flags
  15 Oct 13 -- converting to gm2.
  28 Jun 14 -- converting to Ada.
  31 Aug 14 -- found that tknrtnsa does not process gettknreal correctly.  Had to rewrite it to match the Modula-2 version.
   2 Sep 14 -- Added ToHex back.
   1 Dec 14 -- Converted to cpp.
  20 Dec 14 -- Added use of macros for date of last compile
  31 Dec 14 -- Started coding HOL command, and changed the limits for the OutputFixedOrFloat proc to print Julian numbers in fixed format.
  10 Jan 15 -- Coding use of string operations to supplement output of stack.  And number cropping, addcommas.
   5 Nov 15 -- New commands for recip, curt, vol.
  22 Nov 15 -- Noticed that T1 and T2 stack operations are not correct.  This effects HP2cursed and rpnc.  Changed ARRAYOF Stack declaration.
                Was Stack[T1], now is Stack[StackSize].  The declaration is number of elements, not the high bound.
  13 Apr 16 -- Adding undo and redo commands, which operate on the entire stack not just X register.
  19 May 16 -- Fixing help text for the % commanded coded in 2006.  Oddly enough, the help never included it.
   2 Jul 16 -- Fixed help text for the PI command.  And changed the pivot for the JUL command to be the current year instead of the constant "30".
   7 Jul 16 -- Added UP command.  Surprising I had not done this earlier.
*/

/*
 Declared in the header file
  enum StackRegNames {X,Y,Z,T5,T4,T3,T2,T1};
  const int StackSize = T1 + 1;
*/


ARRAYOF  double Stack[StackSize];
ARRAYOF  double StackMatrix[StackSize][StackSize];   // new matrix to hold the stacks.  Does not need to be a square matrix, but it is easier for me this way.
  const double PI = 3.141592653589793;
  double LastX=0.0;
  double MemReg=0.0;
  HolType Holidays;


//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
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
//  FOR S = Z; S < T1; ++S DO
//    Stack[S-1] = Stack[S];
//  ENDFOR;
//ENDFUNC; // STACKDN
//------------------------------------------------------
PROCEDURE STACKDN() IS
  int S;
  FOR S = Y; S < T1; ++S DO
    Stack[S] = Stack[S+1];
  ENDFOR;
ENDFUNC; // STACKDN
//------------------------------------------------------
PROCEDURE STACKROLLDN() IS
  double TEMP;
  TEMP = Stack[X];
  Stack[X] = Stack[Y];
  STACKDN();
  Stack[T1] = TEMP;
ENDFUNC; // STACKROLLDN
//------------------------------------------------------
PROCEDURE PUSHX(double R) IS
  STACKUP();
  Stack[X] = R;
END; //
//------------------------------------------------------
RETURN double FUNCTION READX() IS
  return Stack[X];
END; // READX
//------------------------------------------------------
PROCEDURE SWAPXY() IS
  double TEMP;
  TEMP = Stack[X];
  Stack[X] = Stack[Y];
  Stack[Y] = TEMP;
END; // SWAPXY
//------------------------------------------------------
PROCEDURE GETSTACK(double STK[]) IS
  int S;
  FOR S = X; S <= T1; ++S DO
    STK[S] = Stack[S];
  ENDFOR;
END;// GETSTACK
//-----------------------------------------------------------------------------------------------------------------------------
RETURN string FUNCTION AddCommas(string instr) IS
  string outstr;
  int len,i,decptposn,NumberOfCommas;

  len = instr.length();
  decptposn = instr.find('.');
  IF (unsigned) decptposn EQ string::npos THEN  // decimal point not found
    instr = instr.append(".");
    i = len;
  ELSE
    i = decptposn;
  ENDIF;
  
  outstr = instr;
  NumberOfCommas = i/3;
  WHILE (NumberOfCommas > 0) AND (i > 3 ) DO
    i -= 3;
    outstr = outstr.insert(i,",");     // insert single comma declared as a c-string.
    --NumberOfCommas;
  ENDWHILE;
  return outstr;
ENDFUNC; // AddCommas
//-----------------------------------------------------------------------------------------------------------------------------
RETURN string FUNCTION CropNStr(string instr) IS
  string outstr;
  int len,i;

  len = instr.length() - 1;
  i = len;
  WHILE (i >= 0) AND (instr.at(i) EQ '0') DO
    --i;
  ENDWHILE;
  outstr = instr;
  IF i < len THEN
    outstr.erase(i+1);  // erases from i-th position to the end of the string.
  ENDIF;

  i = 0;
  len = outstr.length();
  WHILE (i <= len) AND (outstr.at(i) EQ ' ') DO
    ++i;
  ENDWHILE;
  IF i > 0 THEN
    outstr.erase(0,i);  // erases from the beginning to the i-th position.
  ENDIF

  return outstr;
ENDFUNC; // CropNStr

//-----------------------------------------------------------------------------------------------------------------------------
PROCEDURE DumpStackFixed() IS
  const string HeaderDivider = "+------------------------------+";
  const string SpaceFiller = "          ";
  int S;
  string str;
  cout << HeaderDivider  << endl;
  cout.setf(ios::fixed);
  cout.width(15);
  cout.precision(4);
  cout << left << Stack[T1];
  str = to_string(Stack[T1]);
  str = CropNStr(str);
  IF Stack[T1] > 10000 THEN
    str = AddCommas(str);
  ENDIF;
  cout << SpaceFiller << str;
  cout << endl;
  FOR S=T2; S>=X; --S DO
    cout << HeaderDivider << endl;
    cout.width(15);
    cout << left << Stack[S];
    str = to_string(Stack[S]);
    str = CropNStr(str);
    IF Stack[S] > 10000 THEN
      str = AddCommas(str);
    ENDIF;
    cout << SpaceFiller << str;
    cout  << endl;
  ENDFOR;
  cout << HeaderDivider << endl;
END; // DumpStackFixed
// ***************************************************************************
PROCEDURE DumpStackFloat() IS
  const string HeaderDivider = "+------------------------------+";
  const string SpaceFiller = "          ";
  int S;
  string str;
  cout << HeaderDivider << endl;
  cout.setf(ios::scientific);
  cout.width(15);
  cout.precision(10);
  cout << left << Stack[T1];
  str = to_string(Stack[T1]);
  str = CropNStr(str);
  IF Stack[T1] > 10000 THEN
    str = AddCommas(str);
  ENDIF;
  cout << SpaceFiller << str;
  cout << endl;
  FOR S=T2; S>=X; --S DO
    cout << HeaderDivider << endl;
    cout.width(15);
    cout << left << Stack[S]; 
    str = to_string(Stack[S]);
    str = CropNStr(str);
    IF Stack[S] > 10000 THEN
      str = AddCommas(str);
    ENDIF;
    cout << SpaceFiller << str;
    cout << endl;
  ENDFOR;
  cout << HeaderDivider << endl;
END;// DumpStackFloat
//********************************************************************************
PROCEDURE OutputFixedOrFloat(double r) IS
  IF (r EQ 0.0) OR abs(r) < 1.0e-10 THEN  // write 0.0
    cout << "0.0";
  ELSIF (abs(r) > 1.0e-9) AND (abs(r) < 1.0e10) THEN  // write in fixed format
    cout.setf(ios::fixed);
    cout.width(15);
    cout.precision(4);
    cout << left << r;
  ELSE                                                // write in scientific format
    cout.setf(ios::scientific);
    cout.width(15);
    cout.precision(10);
    cout << left << r;
  ENDIF;
END; // OutputFixedOrFloat
//*****************************************************************************
PROCEDURE DumpStackGeneral() IS
  const string HeaderDivider = "+------------------------------+";
  const string SpaceFiller = "          ";
  int S;
  string str;
  cout << HeaderDivider << endl;
  OutputFixedOrFloat(Stack[T1]);
  str = to_string(Stack[T1]);
  str = CropNStr(str);
  IF Stack[T1] > 10000 THEN
    str = AddCommas(str);
  ENDIF;
  cout << SpaceFiller << str;
  cout << endl;
  FOR S=T2; S>=X; --S DO
    cout <<HeaderDivider << endl;
    OutputFixedOrFloat(Stack[S]);
    str = to_string(Stack[S]);
    str = CropNStr(str);
    IF Stack[S] > 10000 THEN
      str = AddCommas(str);
    ENDIF;
    cout << SpaceFiller << str;
    cout << endl;
  ENDFOR;
  cout << HeaderDivider << endl;
END;// DumpStackGeneral

//-------------------------------------------------------------------

RETURN string FUNCTION ToHex(double L)  IS
  int K,Leng;
  unsigned int H;
  bool IsNeg;
  const char OrdZero='0';
  const char OrdCapA='A';
  string u_str1="",u_str2="";

  IF L < 0 THEN
    IsNeg = true;
    H = -L;
  ELSE
    IsNeg = false;
    H = L;
  ENDIF;

  REPEAT
    K = H MOD 16;
    IF (K <= 9) THEN
      u_str1 += (K + OrdZero);
    ELSE
      u_str1 += (K-10 + OrdCapA);
    ENDIF;
    H = H / 16;
  UNTIL(H EQ 0);

  IF IsNeg THEN
    u_str2 += '-';
  ENDIF;

  Leng = u_str1.length()-1;
  WHILE Leng >= 0 DO
    u_str2 += u_str1.at(Leng);
    --Leng;
  ENDWHILE;
  return u_str2;
END;// ToHex
//--------------------------------------------------------
PROCEDURE StacksUp() IS
  int i,j;
  FOR i=T2; i >= 0; i-- DO
    FOR j=X; j <= T1; j++ DO
      StackMatrix[i+1][j] = StackMatrix[i][j];
    ENDFOR; // FOR j
  ENDFOR;  // FOR i
ENDPROC; // StacksUp
//--------------------------------------------------------
PROCEDURE StacksDown() IS
  int i,j;
  FOR i=Y; i <= T1; i++ DO
    FOR j=0; j <= T1; j++ DO
      StackMatrix[i-1][j] = StackMatrix[i][j];
    ENDFOR; // FOR j
  ENDFOR;  // FOR i
ENDPROC; // StacksDown
//--------------------------------------------------------
PROCEDURE PushStacks() IS
  StacksUp();
  int j;
  FOR j=X; j <= T1; j++ DO
    StackMatrix[0][j] = Stack[j];
  ENDFOR;
ENDPROC; // PushStacks
//--------------------------------------------------------
PROCEDURE RollDownStacks() IS    // undo operation for main stack
  int j;
  ARRAYOF  double TempStack[StackSize];

  FOR j=0; j <= T1; j++ DO
    TempStack[j] = Stack[j];
    Stack[j] = StackMatrix[0][j]; 
  ENDFOR;

  StacksDown();

  FOR j=0; j <= T1; j++ DO
    StackMatrix[T1][j] = TempStack[j]; 
  ENDFOR;
ENDPROC; // RollDownStacks  IE undo
//--------------------------------------------------------
PROCEDURE RollUpStacks() IS     // redo uperation for main stack
  int j;
  ARRAYOF  double TempStack[StackSize];

  FOR j=X; j <= T1; j++ DO
    TempStack[j] = Stack[j];
    Stack[j] = StackMatrix[T1][j];
  ENDFOR;

  StacksUp();

  FOR j=X; j <= T1; j++ DO
    StackMatrix[0][j] = TempStack[j];
  ENDFOR;
ENDPROC;  // RollUpStacks  IE redo
//--------------------------------------------------------
//--------------------------------------------------------
//--------------------------------------------------------
RETURN int FUNCTION HCF(int a, int b) IS
// a = bt + r, then hcf(a,b) = hcf(b,r)
  int r,a1,b1;

  IF a < b THEN
    a1 = b;
    b1 = a;
  ELSE
    a1 = a;
    b1 = b;
  ENDIF;
  REPEAT
    r = a1 MOD b1;
    a1 = b1;
    b1 = r;
  UNTIL (r EQ 0);
  return a1;
END;// HCF
//------------------------------------------------------------------------
/*function Trim (Source : in String; Side : in Trim_End) return String;
--          Returns the string obtained by removing from Source all leading
--          Space characters (if Side = Left), all trailing Space characters
--          (if Side = Right), or all leading and trailing Space characters
--          (if Side = Both).

-- procedure Trim (Source : in out String; Side : in Trim_End; Justify : in Alignment := Left; Pad : in Character := Space);
-- Equivalent to Move(Trim(Source, Side), Source, Justify=>Justify, Pad=>Pad).
*/
//-------------------------------------------------------------------------
RETURN double FUNCTION GetResult(string s) IS
    int c,c1,c2,c3;  // these are used for the HCF command, and c3 is for the time arith commands
    int I,year;
    TokenType Token;
    bool EOL;

  
  INITKN(s);
  WHILE true DO //  UNTIL reached EOL
    EOL = GETTKNREAL(Token);
//                                                Put(" In GetResult after GetTknReal and R =");
//                                                Put(R);
//                                           Put(", Token.uStr =" & To_String(Token.uStr) & ", TokenState = ");
//                                                Put(FSATyp'Image(Token.State));
//                                                New_Line;
    IF EOL THEN break; ENDIF;
    I = Token.iSum;
    SWITCH  Token.STATE DO
      case DELIM : break; /* do nothing */
      case DGT : 
                 PUSHX(Token.rSum);
                 PushStacks();
                 break;
      case OP  : IF (I EQ 6) OR (I EQ 20) THEN
      	            SWAPXY();
                 ELSE
                    LastX = Stack[X];
                    PushStacks();
                    SWITCH I DO
                      case  8 : Stack[X] += Stack[Y];
                                 break;
                      case 10 : Stack[X] = Stack[Y] - Stack[X];
                                 break;
                      case 12 : Stack[X] *= Stack[Y];
                                 break;
                      case 14 : Stack[X] = Stack[Y] / Stack[X];
                                 break;
                      case 16 : Stack[X] = pow(Stack[Y], round(Stack[X])); // ^ op
                                 break;
                      case 18 : Stack[X] = pow(abs(Stack[Y]), Stack[X]); // **
                                 break;
                      case 22 : Stack[X] *= Stack[Y] / 100.0;  // percent
                                 break;
                      default :
                                 cerr << Token.uStr + " is an unrecognized operation." << endl;
                                 STACKUP();
                    ENDCASE; // case on opcode
                    IF I NE 22 THEN STACKDN(); ENDIF; // Do not move stack for % operator
                 ENDIF; // opcode value condition
                 break;
    case ALLELSE :  IF false THEN /* do nothing */
                    ELSIF Token.uStr.compare("DUMP") EQ 0 THEN
                      DumpStackGeneral();
                    ELSIF Token.uStr.compare("SHO") EQ 0 THEN
                      DumpStackGeneral();
                    ELSIF Token.uStr.compare("DUMPFIXED") EQ 0 THEN
                      DumpStackFixed();
                    ELSIF Token.uStr.compare("DUMPFLOAT") EQ 0 THEN
                      DumpStackFloat();
                    ELSIF Token.uStr.compare("SQR") EQ 0 THEN
                      LastX = Stack[X];
                      PushStacks();
                      Stack[X] *= Stack[X];
                    ELSIF Token.uStr.compare("SQRT") EQ 0 THEN
                      LastX = Stack[X];
                      PushStacks();
                      Stack[X] = sqrt(Stack[X]);
                    ELSIF Token.uStr.compare("RECIP") EQ 0 THEN
                      LastX = Stack[X];
                      PushStacks();
                      Stack[X] = 1.0/Stack[X];
                    ELSIF Token.uStr.compare("CURT") EQ 0 THEN
                      LastX = Stack[X];
                      PushStacks();
                      Stack[X] = exp(log(Stack[X])/3.0);
                    ELSIF Token.uStr.compare("VOL") EQ 0 THEN
                      LastX = Stack[X];
                      PushStacks();
                      Stack[X] = exp(log(2.0*Stack[X])/3.0);
                    ELSIF Token.uStr.compare("HELP") EQ 0 THEN
                      cout << " SQRT,SQR -- X = sqrt(X) or sqr(X) register." << endl;
                      cout << " CURT -- X = cuberoot(X)." << endl;
                      cout << " RECIP -- X = 1/X." << endl;
                      cout << " VOL -- X = estimated diameter for a given volume, assuming a sphere." << endl;
                      cout << " STO,RCL  -- store/recall the X register to/from the memory register." << endl;
                      cout << " `,~,SWAP,SWAPXY,<>,>< -- equivalent commands that swap the X and Y registers." << endl;
                      cout << " @, LastX -- put the value of the LASTX register back into the X register." << endl;
                      cout << " !,DN,ROLLDN -- roll the stack down one register.  X goes to T1." << endl;
                      cout << " , or UP -- stack up.  ! or DN -- stack down." << endl;
                      cout << " Dump, Dumpfixed, Dumpfloat, Sho -- dump the stack to the terminal." << endl;
                      cout << " EXP,LN -- evaluate exp(X) or ln(X) and put result back into X." << endl;
                      cout << " ^  -- ABS(Y) to the X power, put result in X and pop stack 1 reg.  Rounds X" << endl;
                      cout << " **  -- ABS(Y) to the X power, put result in X and pop stack 1 reg." << endl;
                      cout << " INT, ROUND, FRAC, PI -- do what their names suggest." << endl;
                      cout << " MOD -- evaluate Y MOD X, put result in X and pop stack 1 reg." << endl;
		      cout << " %   -- does XY/100, places result in X.  Leaves Y alone." << endl;
                      cout << " SIN,COS,TAN,ARCTAN,ARCSIN,ARCCOS -- In deg." << endl;
                      cout << " D2R -- perform degrees to radians conversion of the X register." << endl;
                      cout << " R2D -- perform radians to degrees conversion of the X register." << endl;
                      cout << " JUL -- Return Julian date number of Z month, Y day, X year.  Pop stack x2." << endl;
                      cout << " TODAY- Return Julian date number of today's date.  Pop stack x2." << endl;
                      cout << " GREG-- Return Z month, Y day, X year of Julian date number in X." << endl;
                      cout << " DOW -- Return day number 0..6 of julian date number in X register." << endl;
                      cout << " HEX -- Round X register to a long_integer and output it in hex format." << endl;
                      cout << " HCF -- Push HCF(Y,X) onto stack without removing Y or X." << endl;
                      cout << " HOL -- Display holidays." << endl;
                      cout << " UNDO, REDO -- entire stack.  More comprehensive than lastx." << endl;
                      cout << " last compiled " << __DATE__ << "  " <<  __TIME__ << "." << endl;
                    ELSIF Token.uStr.compare("STO") EQ 0 THEN
                      MemReg = Stack[X];
                    ELSIF Token.uStr.compare("RCL") EQ 0 THEN
                      PUSHX(MemReg);
                    ELSIF Token.uStr.compare("UNDO") EQ 0 THEN
                      RollDownStacks();
                    ELSIF Token.uStr.compare("REDO") EQ 0 THEN
                      RollUpStacks();
                    ELSIF Token.uStr.compare("SWAP") EQ 0 THEN
                      PushStacks();
                      SWAPXY();
                    ELSIF Token.uStr.compare("SWAPXY") EQ 0 THEN
                      PushStacks();
                      SWAPXY();
                    ELSIF Token.uStr.compare("LASTX") EQ 0 THEN
                      PushStacks();
                      PUSHX(LastX);
                    ELSIF Token.uStr.compare("ROLLDN")  EQ 0 THEN
                      PushStacks();
                      STACKROLLDN();
                    ELSIF Token.uStr.compare("DN")  EQ 0 THEN
                      PushStacks();
                      STACKROLLDN();
                    ELSIF (Token.uStr.compare(",") EQ 0) OR (Token.uStr.compare("UP") EQ 0) THEN
                      PushStacks();
                      STACKUP();
                    ELSIF Token.uStr.compare("!") EQ 0  THEN
                      PushStacks();
                      Stack[X] = Stack[Y];
                      STACKDN();
                    ELSIF Token.uStr.compare("`") EQ 0  THEN
                      PushStacks();
                      SWAPXY();
                    ELSIF Token.uStr.compare("~") EQ 0  THEN
                      PushStacks();
                      SWAPXY();
                    ELSIF Token.uStr.compare("@") EQ 0  THEN
                      PushStacks();
                      PUSHX(LastX);
                    ELSIF Token.uStr.compare("EXP") EQ 0  THEN
                      PushStacks();
                      LastX = Stack[X];
                      Stack[X] = exp(Stack[X]);
                    ELSIF Token.uStr.compare("LN") EQ 0  THEN
                      PushStacks();
                      LastX = Stack[X];
                      Stack[X] = log(abs(Stack[X]));  // ln in Modula-2
                    ELSIF Token.uStr.compare("Y^X") EQ 0  THEN // Now that ^ is an operator, this must be quoted to be parsed as such
                      PushStacks();
                      LastX = Stack[X];
                      Stack[X] = exp(Stack[X]*log(abs(Stack[Y])));
                      STACKDN();
                    ELSIF Token.uStr.compare("INT") EQ 0  THEN
                      PushStacks();
                      LastX = Stack[X];
                      Stack[X] = trunc(Stack[X]);
                    ELSIF Token.uStr.compare("TRUNC") EQ 0  THEN
                      PushStacks();
                      LastX = Stack[X];
                      Stack[X] = trunc(Stack[X]);
                    ELSIF Token.uStr.compare("ROUND") EQ 0  THEN
                      PushStacks();
                      LastX = Stack[X];
                      Stack[X] = round(LastX);
                    ELSIF Token.uStr.compare("HEX") EQ 0  THEN
                      IF (Stack[X] >= -2.0E9) AND (Stack[X] <= 1.80E19) THEN
                        cout << " Value of X reg in hex: " + ToHex(Stack[X]) << endl;
                      ELSE
                        cerr << " Cannot convert X register to hex string, as number is out of range." << endl;
                      ENDIF; // Hex command
                    ELSIF Token.uStr.compare("HCF") EQ 0  THEN
                      PushStacks();
                      c1 = abs(round(Stack[X]));
                      c2 = abs(round(Stack[Y]));
                      c  = HCF(c2,c1);
                      STACKUP();
                      Stack[X] = c;
                    ELSIF Token.uStr.compare("P") EQ 0  THEN
                      //  essentially do nothing but print RESULT= line again.
                    ELSIF Token.uStr.compare("FRAC") EQ 0  THEN
                      PushStacks();
                      LastX = Stack[X];
                      Stack[X] -= trunc(Stack[X]);
                    ELSIF Token.uStr.compare("MOD") EQ 0  THEN
                      PushStacks();
                      LastX = Stack[X];
                      Stack[X] = abs(lround(Stack[Y])) MOD abs(lround(Stack[X]));
                      STACKDN();
                    ELSIF Token.uStr.compare("SIN") EQ 0  THEN
                      PushStacks();
                      LastX = Stack[X];
                      Stack[X] = sin(Stack[X]*PI/180.0);
                    ELSIF Token.uStr.compare("COS") EQ 0  THEN
                      PushStacks();
                      LastX = Stack[X];
                      Stack[X] = cos(Stack[X]*PI/180.0);
                    ELSIF Token.uStr.compare("TAN") EQ 0  THEN
                      PushStacks();
                      LastX = Stack[X];
                      Stack[X] = tan(Stack[X]*PI/180.0);
                    ELSIF Token.uStr.compare("ARCTAN") EQ 0  THEN
                      PushStacks();
                      LastX = Stack[X];
                      Stack[X] = atan(Stack[X])*180.0/PI;
                    ELSIF Token.uStr.compare("ARCSIN") EQ 0  THEN
                      PushStacks();
                      LastX = Stack[X];
                      Stack[X] = asin(LastX)*180.0/PI;
                    ELSIF Token.uStr.compare("ARCCOS") EQ 0 THEN
                      PushStacks();
                      LastX = Stack[X];
                      Stack[X] = acos(LastX)*180.0/PI;
                    ELSIF Token.uStr.compare("D2R") EQ 0 THEN
                      PushStacks();
                      LastX = Stack[X];
                      Stack[X] *= PI/180.0;
                    ELSIF Token.uStr.compare("R2D") EQ 0 THEN
                      PushStacks();
                      LastX = Stack[X];
                      Stack[X] *= 180.0/PI;
                    ELSIF Token.uStr.compare("JUL") EQ 0 THEN
                      PushStacks();
                      LastX = Stack[X];
                         // allow for 2 digit years
                      TIME2MDY(c1,c2,c3);
//                                                                                               IF Stack[X] <= 30.0 THEN
                      IF Stack[X] <= c3 MOD 100 THEN
                      	Stack[X] += 2000.0;
                      ELSIF Stack[X] < 100.0 THEN
                      	Stack[X] += 1900.0;
                      ENDIF;
                      Stack[X] = JULIAN(lround(Stack[Z]), lround(Stack[Y]), lround(Stack[X]));
                      STACKDN();
                      STACKDN();
                    ELSIF Token.uStr.compare("TODAY") EQ 0 THEN
                      PushStacks();
                      LastX = Stack[X];
                      STACKUP();
                      TIME2MDY(c1,c2,c3);
                      Stack[X] = JULIAN(c1,c2,c3);
                    ELSIF Token.uStr.compare("GREG") EQ 0 THEN
                      PushStacks();
                      LastX = Stack[X];
                      STACKUP();
                      STACKUP();
                      GREGORIAN(round(Stack[X]),c1,c2,c3);
                      Stack[Z] = c1;
                      Stack[Y] = c2;
                      Stack[X] = c3;
                    ELSIF Token.uStr.compare("DOW") EQ 0 THEN
                      PushStacks();
                      LastX = Stack[X];
                      Stack[X] = (lround(LastX) MOD 7);
                    ELSIF Token.uStr.compare("PI") EQ 0 THEN
                      PushStacks();
                      PUSHX(PI);
                    ELSIF Token.uStr.compare("HOL") EQ 0 THEN
                      PushStacks();
                      year = round(Stack[X]);
                      IF year < 40 THEN
                        year += 2000;
                      ELSIF year < 100 THEN
                        year += 1900;
                      ENDIF;
                      IF (year >= 1900) AND (year <= 2100) THEN
                        Holidays.year = year;
                        GetHolidays(Holidays);
                        Holidays.valid = true;
                      ENDIF;
                    ELSIF Token.uStr.compare("ABOUT") EQ 0 THEN
                       // do nothing, but don't generate an error
                    ELSE
                      cerr << Token.uStr + " is an unrecognized command." << endl;
                    ENDIF;  // main text command selection if statement
    ENDCASE;
  ENDWHILE;
  return Stack[X];
END; // GETRESULT
