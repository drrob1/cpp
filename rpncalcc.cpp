-- (C) 1990.  Robert W. Solomon.  All rights reserved.
with Ada; use Ada; -- Before I had this line, I was getting a compilation error saying that "Strings" is not visible.
with Ada.Text_IO; use Ada.Text_IO;
with Ada.Integer_Text_IO; use Ada.Integer_Text_IO;
with Ada.Command_Line; use Ada.Command_Line;
with Ada.Characters; use Ada.Characters;
with Ada.Characters.Conversions;  use Ada.Characters.Conversions;
with Ada.Characters.Handling; use Ada.Characters.Handling;
with Ada.Characters.Latin_1; use Ada.Characters.Latin_1;
with Ada.Strings; use Ada.Strings;
with Ada.Strings.Fixed; use Ada.Strings.Fixed;
with Ada.Strings.Unbounded; use Ada.Strings.Unbounded;
--                                                      with Ada.Strings.Bounded;
with tknrtnsa; use tknrtnsa;


Package hpcalca is
--  This module simulates an RPN calculator, but with an 8 reg stack
--  instead of 4.  All operations occur at the bottom of the stack
function CAP(Item : Character) return Character renames Ada.Characters.Handling.To_Upper;
function CAP(Item : String) return String renames Ada.Characters.Handling.To_Upper;

-- subTYPE String255Fixedtype is String(1..255);  attempt to use the definition in tknsrtna.

Type StackRegNames is (X,Y,Z,T5,T4,T3,T2,T1);
Type RealStack is ARRAY (StackRegNames) OF LONG_FLOAT;

  STACKSIZE : Constant := StackRegNames'Pos(T1) + 1;

Procedure OutputFixedOrFloat(r: Long_Float);
PROCEDURE PUSHX(R : LONG_Float);
Function READX return LONG_Float;
PROCEDURE GETSTACK(STK : out RealStack);
-- PROCEDURE GETSTACK(STK : out ARRAY (Positive range <>) of Long_Float);
-- PROCEDURE GETSTACK(STK : out Positive range <> of Long_Float);
-- PROCEDURE GETSTACK(STK : out RealStack; RETCOD : out Natural);
PROCEDURE DumpStackFloat;
PROCEDURE DumpStackFixed;
PROCEDURE DumpStackGeneral;
function tokenEqualString(tkn : TokenType; str : String)  return boolean;
Function GETRESULT(s : String) return LONG_Float;

END hpcalca;
-- (C) 1990.  Robert W. Solomon.  All rights reserved.
--  REVISION HISTORY
--------------------
--   1 Dec 89 -- Added the help command.
--  24 Dec 91 -- Converted to M-2 V 4.00.  Also changed the params to the
--                GETRESULT proc to be more reasonable.
--  21 Mar 93 -- Added exponentiation and MOD, INT, FRAC and ROUND, as well as used the UL2 procs again.
--  25 Jul 93 -- Added JUL and GREG commands.
--  18 May 03 -- Win32 version using Stony Brook Modula-2 v 4
--  26 May 03 -- Allowed years to pivot for 2000 or 1900 in TIMLIB.
--   4 Oct 03 -- Added LongCard2HexStr and its cmd.
--  31 Oct 03 -- Added HCF cmd.
--   1 Nov 03 -- Fixed the var swap bug in HCF rtn.
--  27 Nov 04 -- Added pi as a cmd.
--  12 Mar 06 -- Made trig functions use arguments in degrees, changed stacksize to const, and moved stackregnames to def.
--  22 Jul 06 -- Added % operator does xy/100 but does not drop stack.  Just like old HP 25.
--  15 Apr 07 -- Added comma to push stack.  Removed it as a delim char and made it a ALLELSE.  And
--                added ! ` and ~ as stack commands.  And updated i/o procedure use.
--   1 Apr 13 -- Modified the DUMP routine so the characters are not printed on the same line as the numbers.  The
--                alignment was all wrong anyway.  And added Dump2Console.
--   2 May 13 -- Will use consolemode compilation flags
--  15 Oct 13 -- converting to gm2.
--  28 Jun 14 -- converting to Ada.
--  31 Aug 14 -- found that tknrtnsa does not process gettknreal correctly.  Had to rewrite it to match the Modula-2 version.
--   2 Sep 14 -- Added ToHex back.

-- with Ada; use Ada; -- Before I had this line, I was getting a compilation error saying that "Strings" is not visible.
-- with Ada.Text_IO; use Ada.Text_IO;
-- with Ada.Integer_Text_IO; use Ada.Integer_Text_IO;
with Ada.Long_Float_Text_IO; use Ada.Long_Float_Text_IO;
-- with Ada.Command_Line; use Ada.Command_Line;
-- with Ada.Characters; use Ada.Characters;
-- with Ada.Characters.Conversions;  use Ada.Characters.Conversions;
-- with Ada.Characters.Handling; use Ada.Characters.Handling;
-- with Ada.Characters.Latin_1; use Ada.Characters.Latin_1;
-- with Ada.Strings; use Ada.Strings;
-- with Ada.Strings.Fixed; use Ada.Strings.Fixed;
-- with Ada.Strings.Unbounded; use Ada.Strings.Unbounded;
--                                                                          with Ada.Strings.Bounded;
with Ada.Strings.Maps; use Ada.Strings.Maps;
with Ada.Numerics; use Ada.Numerics;  -- has Ada.Numerics.e and Ada.Numerics.pi
with Ada.Numerics.Long_Elementary_Functions; use Ada.Numerics.Long_Elementary_Functions;
------------------------------
-- with tknrtnsa; use tknrtnsa;
-- with environa; use environa;
with timliba; use timliba;
------------------------------
Package body hpcalca is
-- Declared in the spec file.
-- Type StackRegNames is (X,Y,Z,T5,T4,T3,T2,T1);
-- Type RealStack is ARRAY (StackRegNames) OF LONG_FLOAT;
-- STACKSIZE : Constant := StackRegNames'Pos(T1) + 1;
-- CONST PI        = 3.141592653589793;
-- subTYPE String255Fixedtype is String(1..255);


  SEMCOL    : constant Character := ';';  -- (* 59 *)

  LastX,MemReg : LONG_Float;
  Stack        : RealStack;
--  S            : StackRegNames;  enumeration type of stack reg names x .. T1
--  C,C1,C2,C3   : Natural;
  str          : tknrtnsa.String255Fixedtype;
  uStr         : Unbounded_String;
  bool,EOL     : BOOLEAN;
  stack_too_small : Exception;
  Token : TokenType;
  delim_sequence : constant Character_Sequence := " ;";
------------------------------------------------------
PROCEDURE STACKUP is
BEGIN
  FOR S in reverse StackRegNames range X .. T2 Loop
    STACK(StackRegNames'Succ(S)) := STACK(S);
  END loop;
END STACKUP;
------------------------------------------------------
PROCEDURE STACKDN is
BEGIN
  FOR S in StackRegNames range Z .. T1 Loop
    STACK(StackRegNames'Pred(S)) := STACK(S);
  END loop;
END STACKDN;
------------------------------------------------------
PROCEDURE STACKROLLDN is
    TEMP : LONG_Float;
BEGIN
  TEMP := STACK(X);
  STACK(X) := STACK(Y);
  STACKDN;
  STACK(T1) := TEMP;
END STACKROLLDN;
------------------------------------------------------
PROCEDURE PUSHX(R : LONG_Float) is
BEGIN
  STACKUP;
  STACK(X) := R;
END PUSHX;
------------------------------------------------------
Function READX return LONG_Float is
BEGIN
  RETURN STACK(X);
END READX;
------------------------------------------------------
PROCEDURE SWAPXY is
  TEMP : LONG_Float;
BEGIN
  TEMP := STACK(X);
  STACK(X) := STACK(Y);
  STACK(Y) := TEMP;
END SWAPXY;
------------------------------------------------------
--PROCEDURE GETSTACK(STK : out ARRAY (Positive range <>) of Long_Float);
PROCEDURE GETSTACK(STK : out RealStack) is
--    S : StackRegNames;
BEGIN
--  RETCOD := 0;
--  IF STK'Last + 1 < STACKSIZE THEN
--  Output param not large enough to hold entire stack.  Remember HIGH
--  proc returns the highest subscript of a zero origin array.
--    Raise stack_too_small;
--  END IF;
  FOR S in StackRegNames Loop
    STK(S) := STACK(S);       -- just to see if this will compile.
--                                                            STK(StackRegNames'Pos(S)) := STACK(S);
  END loop;
  Stk := Stack;  -- just to see if this will compile.
END GETSTACK;
-----------------------------------------------------------------------------------------------------------------------------
-- Long_Float_Text_IO -> Fore.Aft  or Fore.Aft E Exp
-- without any spaces between these fields.  The Fore field may includeleading spaces, and a minus sign for negative values.
-- The Aft field includes only decimal digits (possibly with trailing zeros).  The Exp field includes the
-- sign (plus or minus) and the exponent (possibly with leading zeros).
-- For floating point types, the default lengths of these fields are defined
-- by the following variables that are declared in the generic package Float_IO:
-- Default_Fore : Field := 2; Default_Aft  : Field := Num'Digits-1; Default_Exp  : Field := 3;
-- For ordinary or decimal fixed point types, the default lengths of these fields are defined by the following variables
-- that are declared in the generic packages Fixed_IO and Decimal_IO, respectively:
-- Default_Fore : Field := Num'Fore; Default_Aft : Field := Num'Aft; Default_Exp : Field := 0;
-- procedure Put(File : in File_Type; Item : in Num; Fore : in Field := Default_Fore; Aft : in Field := Default_Aft; Exp : in Field := Default_Exp);
-- procedure Put(Item : in Num; Fore : in Field := Default_Fore; Aft : in Field := Default_Aft; Exp : in Field := Default_Exp);
PROCEDURE DUMPSTACKfixed is
--    S      : StackRegNames;
    HeaderDivider : constant String := "+--------------------+";
    SpaceFiller : constant String := "          ";
BEGIN
  Put_Line(HeaderDivider & SpaceFiller & HeaderDivider);
  Put("  ");
  Put(Stack(T1),9,6,0);
  New_Line;
  FOR S in reverse StackRegNames range X .. T2 Loop
    Put_Line(HeaderDivider & SpaceFiller & HeaderDivider);
    Put("  ");
    Put(STACK(S),9,6,0);
    New_Line;
  END loop;
  Put_Line(HeaderDivider & SpaceFiller & HeaderDivider);
END DUMPSTACKfixed;

PROCEDURE DumpStackFloat is
    HeaderDivider : constant String := "+--------------------+";
    SpaceFiller : constant String := "          ";
BEGIN
  Put_Line(HeaderDivider & SpaceFiller & HeaderDivider);
  Put("   ");
  Put(Stack(T1));
  New_Line;
  FOR S in reverse StackRegNames range X .. T2 loop
    Put_Line(HeaderDivider & SpaceFiller & HeaderDivider);
    Put("   ");
    Put(Stack(S));
    New_Line;
  END loop;
  Put_Line(HeaderDivider & SpaceFiller & HeaderDivider);
END DumpStackFloat;

Procedure OutputFixedOrFloat(r: Long_Float) is
BEGIN
  IF (r = 0.0) OR ABS(r) < 1.0e-10 THEN
    Put(0.0,1,1,0);
  ELSIF (ABS(r) > 1.0e-9) AND (ABS(r) < 1.0e10) THEN
    Put(r,9,6,0);
  ELSE
    Put(r);
  END IF;
END OutputFixedOrFloat;

PROCEDURE DumpStackGeneral is
    HeaderDivider : constant String := "+--------------------+";
    SpaceFiller : constant String := "          ";
BEGIN
  Put_Line(HeaderDivider & SpaceFiller & HeaderDivider);
  Put("   ");
  OutputFixedOrFloat(Stack(T1));
  New_Line;
--                                                                       FOR S := T2 TO X BY -1 DO
  FOR S in reverse StackRegNames range X .. T2 loop
    Put_Line(HeaderDivider & SpaceFiller & HeaderDivider);
    Put("   ");
    OutputFixedOrFloat(Stack(S));
    New_Line;
  END loop;
  Put_Line(HeaderDivider & SpaceFiller & HeaderDivider);
END DumpStackGeneral;
-------------------------------------------------------------------

FUNCTION ToHex(L : Long_Integer) RETURN STRING is
  H,K,Leng  : Integer;
  IsNeg     : Boolean;
  dgtval    : Natural;
  dgtchar   : Character;
  ZeroOrd   : constant := Character'Pos('0');
  CapAord   : constant := Character'Pos('A');
  u_str1,u_str2 : unbounded_string := tknrtnsa.NulluStr;

BEGIN
  IF L < 0 THEN
    IsNeg := TRUE;
    H := Integer(-L);
  ELSE
    IsNeg := FALSE;
    H := Integer(L);
  END IF;

  LOOP 
    K := H MOD 16;
    IF (K <= 9) THEN
      u_str1 := u_Str1 & Character'Val(K + ZeroOrd);
    ELSE 
      u_str1 := u_Str1 & Character'Val(K-10 + CapAord);
    END IF;
    H := H / 16;
    EXIT WHEN H = 0;
  END LOOP;

  Leng := Length(u_str1);
--                                                     Move(To_String(u_str1),str);
--                                                     Put_Line(" In ToHex.  u_str1 is: " & str(1..leng) & ".");
  IF IsNeg THEN
    u_str2 := u_str2 & '-';
  END IF;

  WHILE Leng > 0 Loop
    u_str2 := u_str2 & Element(u_str1,Leng);
    Leng := Leng - 1;
  END LOOP;
--                                           Move(To_String(u_str2),str);
--                                           Put_Line(" In Tohex.  u_str2 is: " & str(1..Length(u_str2)) & ".");
  RETURN To_String(u_str2);
END ToHex;


--------------------------------------------------------
function HCF(a,b : in Positive) return Natural is
-- a = bt + r, then hcf(a,b) = hcf(b,r)
  r,a1,b1 : Natural;
BEGIN
  IF a < b THEN
    a1 := b;
    b1 := a;
  ELSE
    a1 := a;
    b1 := b;
  END if;
  Loop
    r := a1 MOD b1;
    a1 := b1;
    b1 := r;
    Exit When r = 0;
  end loop;
  RETURN a1;
END HCF;
------------------------------------------------------------------------
-- function Trim (Source : in String; Side : in Trim_End) return String;
--          Returns the string obtained by removing from Source all leading
--          Space characters (if Side = Left), all trailing Space characters
--          (if Side = Right), or all leading and trailing Space characters
--          (if Side = Both).

-- procedure Trim (Source : in out String; Side : in Trim_End; Justify : in Alignment := Left; Pad : in Character := Space);
-- Equivalent to Move(Trim(Source, Side), Source, Justify=>Justify, Pad=>Pad).
-------------------------------------------------------------------------
function tokenEqualString(tkn : TokenType; str : String)  return boolean is
--  str    : String255Fixedtype;
  ustr,ustr2     : unbounded_string;
  STRLEN,StrLen2 : Natural;
 
BEGIN
--                                                      Put(" In tokenEqualString before TRIM, token and str are:" & To_String(tkn.uStr) & ", " & str );
  
  uStr := TRIM(tkn.uStr,Side => Both);
  StrLen := Length(tkn.uStr);
  uStr2 := TRIM(To_Unbounded_String(Str),Side => Both);
  StrLen2 := Length(uStr);
--                                                      Put(" In tokenEqualString after TRIM, token and str are:" & To_String(tkn.uStr) & ", " & str );
--                                                      New_Line;
  IF ustr = ustr2 Then
    return true;
  Else
    return false;
  end if;
End tokenEqualString; 
-----------------------------------------------------
function GetResult(s : String) return LONG_Float is
    c,c1,c2,c3  : Natural;  -- these are used for the HCF command, and c3 is for the time arith commands
    I           : INTEGER;
    TOKEN       : TokenType;
    R           : LONG_Float;
    L           : LONG_INTeger;
BEGIN
                                                             -- Buf := TRIM(BUF,side=>both);
--                                                Put(" in GetResult before GetTknReal and input string is: ");
--                                                Put_Line(s);
  INITKNCalc(s);
  LOOP --  UNTIL FINISHED, I.E., reached EOL
    GETTKNREAL(TOKEN,R,EOL);
--                                                Put(" In GetResult after GetTknReal and R =");
--                                                Put(R);
--                                           Put(", Token.uStr =" & To_String(Token.uStr) & ", TokenState = ");
--                                                Put(FSATyp'Image(Token.State));
--                                                New_Line;
    IF EOL THEN EXIT; END IF;
    I := Token.Sum;
    CASE Token.state is
      When Delim => Null;
      When DGT => PUSHX(R);
      When OP  => IF (I = 6) OR (I = 20) THEN
      	            SWAPXY;
                  ELSE
                    LASTX := STACK(X);
                    CASE I is
                      when  8 => STACK(X) := STACK(Y) + STACK(X);
                      when 10 => STACK(X) := STACK(Y) - STACK(X);
                      when 12 => STACK(X) := STACK(Y) * STACK(X);
                      when 14 => STACK(X) := STACK(Y) / STACK(X);
                      when 16 => STACK(X) := (abs STACK(Y)) ** Long_Float'Rounding(STACK(X)); -- ^
                      when 18 => STACK(X) := (abs STACK(Y)) ** STACK(X); -- **
                      when 22 => STACK(X) := STACK(Y) * STACK(X) / 100.0;  -- percent
                      when others =>
                                 Put_Line(To_String(TOKEN.uStr) & " is an unrecognized operation.");
                                 STACKUP;
                    END CASE;
                    IF I /= 22 THEN STACKDN; END if; -- Do not move stack for % operator
                  END IF;
    When ALLELSE => IF false THEN Null;
                    ELSIF tokenEqualString(TOKEN,"DUMP") THEN
                      DumpStackGeneral;
                    ELSIF tokenEqualString(TOKEN,"DUMPFIXED") THEN
                      DumpStackFixed;
                    ElsIf tokenEqualString(Token,"DUMPFLOAT") THEN
                      DumpStackFloat;
                    ELSIF tokenEqualString(TOKEN,"SQR") THEN
                      LASTX := STACK(X);
                      STACK(X) := STACK(X) * STACK(X);
                    ELSIF tokenEqualString(TOKEN,"SQRT") THEN
                      LASTX := STACK(X);
                      STACK(X) := sqrt(STACK(X));
                    ELSIF tokenEqualString(TOKEN,"HELP") THEN
                      Put_Line(" SQRT,SQR -- X = sqrt(X) or sqr(X) register.");
                      Put_Line(" STO,RCL  -- store/recall the X register to/from the memory register.");
                      Put_Line(" ~,SWAP,SWAPXY,<>,>< -- equivalent commands that swap the X and Y registers.");
                      Put_Line(" @, LASTX -- put the value of the LASTX register back into the X register.");
                      Put_Line(" !,DN,ROLLDN -- roll the stack down one register.  X goes to T1.");
                      Put_Line(" , -- stack up.  ! -- stack down.");
                      Put_Line(" Dump, Dumpfixed, Dumpfloat -- dump the stack to the terminal.");
                      Put_Line(" EXP,LN -- evaluate exp(X) or ln(X) and put result back into X.");
                      Put_Line(" ^  -- ABS(Y) to the X power, put result in X and pop stack 1 reg.  Uses Natural(X)");
                      Put_Line(" **  -- ABS(Y) to the X power, put result in X and pop stack 1 reg.");
                      Put_Line(" INT, ROUND, FRAC -- do what their names suggest.");
                      Put_Line(" MOD -- evaluate Y MOD X, put result in X and pop stack 1 reg.");
                      Put_Line(" SIN,COS,TAN,ARCTAN,ARCSIN,ARCCOS -- take a wild guess.  In deg.");
                      Put_Line(" D2R -- perform degrees to radians conversion of the X register.");
                      Put_Line(" R2D -- perform radians to degrees conversion of the X register.");
                      Put_Line(" JUL -- Return Julian date number of Z month, Y day, X year.  Pop stack x2.");
                      Put_Line(" TODAY- Return Julian date number of today's date.  Pop stack x2.");
                      Put_Line(" GREG-- Return Z month, Y day, X year of Julian date number in X.");
                      Put_Line(" DOW -- Return day number 1..7 of julian date number in X register.");
                      Put_Line(" HEX -- Round X register to a long_integer and output it in hex format.");
                      Put_Line(" HCF -- Push HCF(Y,X) onto stack without removing Y or X.");
                      Put_Line(" compiled September 2, 2014.");
                    ELSIF tokenEqualString(TOKEN,"STO") THEN
                      MEMREG := STACK(X);
                    ELSIF tokenEqualString(TOKEN,"RCL") THEN
                      PUSHX(MEMREG);
                    ELSIF tokenEqualString(TOKEN,"SWAP") THEN
                      SWAPXY;
                    ELSIF tokenEqualString(TOKEN,"SWAPXY") THEN
                      SWAPXY;
                    ELSIF tokenEqualString(TOKEN,"LASTX") THEN
                      PUSHX(LASTX);
                    ELSIF tokenEqualString(TOKEN,"ROLLDN") THEN
                      STACKROLLDN;
                    ELSIF tokenEqualString(TOKEN,"DN") THEN
                      STACKROLLDN;
                    ELSIF tokenEqualString(TOKEN,",") THEN
                      STACKUP;
                    ELSIF tokenEqualString(TOKEN,"!") THEN
                      STACK(X) := STACK(Y);
                      STACKDN;
                    ELSIF tokenEqualString(TOKEN,"`") THEN
                      SWAPXY;
                    ELSIF tokenEqualString(TOKEN,"~") THEN
                      SWAPXY;
                    ELSIF tokenEqualString(TOKEN,"@") THEN
                      PUSHX(LASTX);
                    ELSIF tokenEqualString(TOKEN,"DUMP") THEN
                      DUMPSTACKfixed;
                    ELSIF tokenEqualString(TOKEN,"EXP") THEN
                      LASTX := STACK(X);
                      STACK(X) := exp(STACK(X));
                    ELSIF tokenEqualString(TOKEN,"LN") THEN
                      LASTX := STACK(X);
                      STACK(X) := log(STACK(X));  -- ln in Modula-2
                    ELSIF tokenEqualString(TOKEN,"Y^X") THEN
-- Now that ^ is an operator, this must be quoted to be parsed as such
                      LASTX := STACK(X);
                      STACK(X) := exp(STACK(X)*log(ABS(STACK(Y))));
                      STACKDN;
                    ELSIF tokenEqualString(TOKEN,"INT") THEN
                      LASTX := STACK(X);
                      STACK(X) := Long_Float'truncation(STACK(X));
                    ELSIF tokenEqualString(TOKEN,"TRUNC") THEN
                      LASTX := STACK(X);
                      STACK(X) := Long_Float'truncation(STACK(X));
                    ELSIF tokenEqualString(TOKEN,"ROUND") THEN
                      LASTX := STACK(X);
                      STACK(X) := Long_Float'Rounding(LastX);
                    ELSIF tokenEqualString(TOKEN,"HEX") THEN
                      IF (STACK(X) >= -2.0E9) AND (STACK(X) <= 1.80E19) THEN
                        L := Long_Integer(STACK(X));
                        PUT_Line(" Value of X reg in hex: " & ToHex(L));
                      ELSE
                        Put(" Cannot convert to hex string, as number is out of range.");
                        New_Line;
                      END if; -- Hex command
                    ELSIF tokenEqualString(TOKEN, "HCF") THEN
                      c1 := Natural(ABS STACK(X));
                      c2 := Natural(ABS STACK(Y));
                      c  := HCF(c2,c1);
                      STACKUP;
                      STACK(X) :=Long_FLOAT(c);
                    ELSIF tokenEqualString(TOKEN, "P") THEN
                      --  essentially do nothing but print RESULT= line again.
                      null;
                    ELSIF tokenEqualString(TOKEN, "FRAC") THEN
                      LASTX := STACK(X);
                      STACK(X) := Long_Float'Fraction(STACK(X));
                    ELSIF tokenEqualString(TOKEN, "MOD") THEN
                      LASTX := STACK(X);
                      Stack(X) := Long_Float(Positive(abs Stack(Y)) MOD Natural(abs Stack(X)));
--                                                             STACK[X] := AMOD(STACK[Y],STACK[X]);
                      STACKDN;
                    ELSIF tokenEqualString(TOKEN, "SIN") THEN
                      LASTX := STACK(X);
                      Stack(X) := Sin(Stack(X),360.0);
--                                                             STACK[X] := sin(STACK[X]*pi/180.0);
                    ELSIF tokenEqualString(TOKEN, "COS") THEN
                      LASTX := STACK(X);
                      Stack(X) := Cos(Stack(X),360.0);
--                                                             STACK[X] := cos(STACK[X]*pi/180.0);
                    ELSIF tokenEqualString(TOKEN, "TAN" ) THEN
                      LASTX := STACK(X);
                      Stack(X) := Tan(Stack(X),360.0);
--                                      STACK[X] := sin(STACK[X]*pi/180.0)/cos(STACK[X]*pi/180.0);
                    ELSIF tokenEqualString(TOKEN, "ARCTAN") THEN
                      LASTX := STACK(X);
                      Stack(X) := ArcTan(Stack(X),360.0);
--                                                             STACK[X] := arctan(STACK[X])*180.0/pi;
                    ELSIF tokenEqualString(TOKEN, "ARCSIN") THEN
                      LASTX := STACK(X);
                      Stack(X) := ArcSin(Stack(X),360.0);
--                                          STACK[X] := arctan(LASTX/sqrt(1.0-(LASTX*LASTX)))*180.0/pi;
                    ELSIF tokenEqualString(TOKEN, "ARCCOS") THEN
                      LASTX := STACK(X);
                      Stack(X) := ArcCos(Stack(X),360.0);
--                                          STACK[X] := arctan(sqrt(1.0-(LASTX*LASTX))/LASTX)*180.0/pi;
                    ELSIF tokenEqualString(TOKEN, "D2R") THEN
                      LASTX := STACK(X);
                      STACK(X) := STACK(X)*pi/180.0;
                    ELSIF tokenEqualString(TOKEN, "R2D") THEN
                      LASTX := STACK(X);
                      STACK(X) := STACK(X)*180.0/pi;
                    ELSIF tokenEqualString(TOKEN, "JUL") THEN
                      LASTX := STACK(X);
-- allow for 2 digit years
                      IF STACK(X) <= 30.0 THEN
                      	STACK(X) := STACK(X) + 2000.0;
                      ElsIf Stack(X) < 100.0 THEN
                      	Stack(X) := Stack(X) + 1900.0;
                      END if;
                      STACK(X) :=Long_FLOAT(JULIAN(Natural(STACK(Z)), Natural(STACK(Y)), Natural(STACK(X))));
                      STACKDN;
                      STACKDN;
                    ELSIF tokenEqualString(TOKEN,"TODAY") THEN
                      LASTX := STACK(X);
                      STACKUP;
                      TIME2MDY(C1,C2,C3);
                      STACK(X) :=Long_FLOAT(JULIAN(C1,C2,C3));
                    ELSIF tokenEqualString(TOKEN,"GREG") THEN
                      LASTX := STACK(X);
                      STACKUP;
                      STACKUP;
                      GREGORIAN(Positive(STACK(X)),C1,C2,C3);
                      STACK(Z) :=Long_FLOAT(C1);
                      STACK(Y) :=Long_FLOAT(C2);
                      STACK(X) :=Long_FLOAT(C3);
                    ELSIF tokenEqualString(TOKEN,"DOW") THEN
                      LASTX := STACK(X);
                      Stack(X) := Long_Float(Integer(LastX) MOD 7 + 1);
--                      STACK[X] := AMOD(LASTX,7.0) + 1.0;
                    ELSIF tokenEqualString(TOKEN,"PI") THEN
                      PUSHX(pi);
                    ELSE
                      Put_Line(To_String(TOKEN.uStr) & " is an unrecognized command.");
                    END IF;  -- main text command selection if statement
    END CASE;
  END LOOP;
  RETURN STACK(X);
END GETRESULT;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
BEGIN
  Stack := (others => 0.0);
--  FOR S := X TO T1 DO STACK[S] := 0.0; END(*FOR*);
--                                                                           (*  STACKSIZE  := ORD(T1) + 1; *)
  LASTX := 0.0;
  MEMREG := 0.0;
  NewDelimSeq(delim_sequence);
--                                    delimSet := CHARSETTYP{ASCII.sp,NULL,SEMCOL};  (* Note absence of COMMA *)
--                                    NEWDELIMSET(delimSet);
END hpcalca;
