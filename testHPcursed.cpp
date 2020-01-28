// (C) 1990-2015.  Robert W. Solomon.  All rights reserved.
#define NCURSESON
#include "macros.h"
#include "hpcalcc.h"   // defines StackRegNames, StackSize and the needed procedures.
#include "getcommandline.h"
#include "tokenizec.h"
#include "timlibc.h"
#include "holidaycalc.h"

/*--------------------------------------
REVISION HISTORY
================
16 Oct 03 -- First full Windows version finished.
18 Oct 03 -- Changed how help cmd handled.
21 Oct 03 -- Changed how output strings are converted.  And added popups.
31 Oct 03 -- Added Prime & HCF.
 1 Nov 03 -- Fixed bug in var swap for hcf cmd.
 4 Nov 03 -- Made Prime use a LongCard.
11 Nov 03 -- Added DOW to here so it can display a day name.  And changed HELP display.  And added Terminal Module stuff.
18 Nov 03 -- Liked Terminal window for help, so will not use TextOut.
23 Nov 03 -- Changed to using TextWindows routines.
28 Nov 03 -- Fixed bug in handling backspace & Del string display.
22 Dec 03 -- Added ability to use clipboard routines.
24 Aug 04 -- Added all the storage registers and file i/o of same.
20 Nov 04 -- Made change in key logic so that '=' becomes '+', not requiring a shift press.
27 Nov 04 -- Did same for '*' by allowing ';' instead.  And added PI as a cmd 2 HPCALC.
28 Nov 04 -- Allowed comma for minus and squote for plus.
 8 Jan 05 -- Playing w/ icons.
17 Jan 05 -- Now playing w/ text characterists and positioning.
 1 Feb 05 -- Playing w/ Window Titles.
 5 Feb 06 -- Playing w/ Menus and added RegAltered flag.
11 Mar 06 -- Saving and restoring stack
 7 Mar 07 -- Discovered an occasional bug in arith in which it seems operators act more than once.
              Changed how characters are written to display.  Hope this helps track down pblm.
              If not, I'll consider repainting screen to see if this helps track pblm down.
15 Apr 07 -- Still haven't a handle on this occasional arith bug.  But added arrow key and other
              stack functions.
25 Apr 07 -- Added terminal window output of typed lines, to be a record of what is typed at cmd line.
26 Apr 07 -- Playing with another window that I want to be a child window.
29 Apr 07 -- Now that I got this extra window thing down, I'm going to try it for the help window.
26 Jun 07 -- Never did help window.  Decided to try calling ProcessInput after a space.
 4 Jul 07 -- Made it not exit on space.
 9 Jul 07 -- Fixed bug in that processinput not called when hit up arrow.  And call processinput after
              unambiguous operators, which does not include '*' or '-'.
 4 Aug 07 -- Fixed minor bug when some terminating operators were not being written to childscreen.
29 Sep 07 -- Will now processinput on '=', and "'" for subtraction.
17 Sep 08 -- Found problem w/ processing cmd like RCL/, etc.  Fixed it.
30 Oct 08 -- Found that ' ' is appended to inputstring, so 'FIX' becomes 'FIX ' and is not recognized.
 1 Feb 09 -- Fixed bug in that menu help different than command help.
 8 Mar 09 -- Added code to immediately process subt operator, making squote redundant.  And I added a
             RETURN(DEFAULT_HANDLE) in that area to return control to Windows and what seems to be exiting
             the program.  I remember reading that the purpose of the windows procedure is so Windows
             has a procedure to call when it gets messages.  It seems to work.  Perhaps I should use this
             trick more widely.  Will it speed up the code?  Who knows if this even matters any more.
 9 Mar 09 -- Made it RETURN(USER_HANDLE);
 7 Jun 11 -- GetCommandLine and push whatever is there onto the stack upon starting.
             rpncalc in linux works this way and that pgm will exit after doing whatever the
             command line says.  If started w/o params, it does not exit imm'ly.
20 Oct 11 -- Noticed that the change that allowed '-' to operate imm'ly prevented negative exponents from
             being entered.  I'll use an underscore to represent the unary '-' negative sign.
             A side benefit to the GetCommandLine change is that neg exponents can be entered this way from
             the commandline.
11 Nov 14 -- Added a window to display the nonzero register contents.  And added a command to zero the registers.
26 Dec 14 -- Added HOL command, and fiddled w/ colors.
 1 Jan 15 -- This ncurses version started, derived from HPWinMenu.
--------------------------------------*/
extern  HolType Holidays;
enum OutStateTyp {FIX,FLOAT,GEN};
OutStateTyp OutState = FIX;
ARRAY double Stk[StackSize];  // this is a copy of the stack which lives in HPCALCC.
const int a = 1;
const int aInverted = 2;
const int aInvertedRed = 3;
const int aInvertedCyan = 4;
const int aInvertedYellow = 5;
const int aInvertedBlue = 6;

/**********************************************************************/
RETURN bool FUNCTION FileExists(CONSTARRAYOFCHAR filename) IS
  FileHandle fh;
  fh = fopen(filename,"r");
  IF fh EQ nullptr THEN
    return false;
  ELSE
    fclose(fh);
    return true;
  ENDIF;
ENDFUNC; // FileExists

/*****************************************************************/

/*****************************************************************/
RETURN bool FUNCTION ODD(long i) IS
  return (i MOD 2) EQ 1;
ENDPROC;

RETURN bool FUNCTION EVEN(long i) IS
  return (i MOD 2) EQ 0;
ENDPROC;
//****************************************************************
RETURN bool FUNCTION IsPrime(long lc) IS

  long t,m;

  IF (lc EQ 0) OR (lc EQ 1) THEN return false;
  ELSIF (lc EQ 2) OR (lc EQ 3) THEN return true;
  ELSIF EVEN(lc) THEN return false;
  END;

  m = round(sqrt( (double) lc));
  t = 3;
  REPEAT
    IF (lc MOD t) EQ 0 THEN 
      return false;
    ENDIF;
    t += 2;
  UNTIL (t > m);
  return true;
ENDPROC; // IsPrime

/*++++*****************************************************************/
// RETURN string FUNCTION ToHex(double L);  already coded and working in hpcalcc.cpp
// PROCEDURE LongCard2HexStr(L : LONGCARD; VAR OutStr : ARRAY OF CHAR);
// PROCEDURE LongInt2HexStr(L : LONGINT; VAR OutStr : ARRAY OF CHAR);

/*++++*****************************************************************/

/********************************************************************/
RETURN int FUNCTION GetRegIdx(char chr) IS
                                            /* Return 0..35 w/ A = 10 and Z = 35 */
  char ch;
  int idx=0;
  int ORDCH;
  const int ASCZERO = '0';

  ch = CAP(chr);
  ORDCH = ch;
  IF (ch >= '0') AND (ch <= '9') THEN
    idx = ORDCH - ASCZERO;
  ELSIF (ch >= 'A') AND (ch <= 'Z') THEN
    idx  = ORDCH - 'A' + 10;
  ENDIF;
  return idx;
ENDFUNC; // GetRegIdx
/********************************************************************/
RETURN char FUNCTION GetRegChar(int idx) IS
                                            /* Return '0'..'Z' with A = 10 and Z = 35 */
  char ch;

  IF (idx >= 0) AND (idx <= 9) THEN
    ch = idx + '0';
  ELSIF (idx >= 10) AND (idx <= 35) THEN
    ch  = idx - 10 + 'A';
  ELSE
    ch  = '0';
  ENDIF;
  return ch;
ENDFUNC; // GetRegChar

/********************************************************************/

int main(argcargv) IS
  char ch;
  int i;
/* this code works
  WHILE true DO
    cout << "Enter a character to convert : ";
    cin >> ch;

    i = GetRegIdx(ch);
    cout << ",  converted value is : " << i << endl;
  ENDWHILE;
*/
/* this code works */
while (true) LOOP
  cout << "Enter a number in range 0..35 : ";
  cin >> i;

  ch = GetRegChar(i);
  cout << "Converted character is : " << ch <<endl;
  IF i > 35 THEN return 0; ENDIF;
END;
/**/
/*
while (true) LOOP
  cout << " Enter a possibly prime : ";
  cin >> i;
  IF i < 0 THEN
    break;
  ELSIF IsPrime(i) THEN
    cout << i << " is prime." << endl;
  ELSE
    cout << i << " is not prime." << endl;
  ENDIF;
ENDWHILE;
*/
  return 0;
END;
