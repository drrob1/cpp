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
18 Jan 15 -- First fully working ncurses version completed.  Now to do some tweaking of output formatting.
22 Jan 15 -- Tweaking output on winright tile
23 Jan 15 -- Fixed that delete cannot delete more characters than are there.
13 Feb 15 -- Divided right window into upper and lower, added scrollok(), leaveok, and used addstr to see how it works.
              Added RegAltered flag to ZeroReg command.
              And changed name to HP2cursed.cpp so as to preserve the original code that works.
14 Feb 15 -- Added cooked mode for help.
--------------------------------------*/
extern  HolType Holidays;
extern const int StackSize;
enum OutStateTyp {FIX,FLOAT,GEN};
OutStateTyp OutState = FIX;
ARRAYOF double Stk[StackSize];  // this is a copy of the stack which lives in HPCALCC.  It was defined as ARRAY [1..STACKSIZE] OF LONGREAL;
ARRAYOF double Reg[36];                    // was ARRAY [0..35] OF LONGREAL;
const int a = 1;
const int aInverted = 2;
const int aInvertedRed = 3;
const int aInvertedCyan = 4;
const int aInvertedYellow = 5;
const int aInvertedBlue = 6;
const int aRed = 7;
const int aCyan = 8;
const int aYellow = 9;
const int aBlue = 10;
const int KEY_RETURN = 13;  // I prefer decimal notation for these key codes.
const int KEY_ESC = 27;    // I prefer decimal notation for these key codes.
const int KEY_DEL = 330;  // I prefer decimal notation for these key codes.
const int KEY_TAB = 9;   // I prefer decimal notation for these key codes.
bool inputprocessed = false;
bool RegAltered = false;
string inputline;
int sigfig=2;
CONSTARRAYOFCHAR DayNames[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
ARRAYOF string MonthNames[13] = {"filler","January","February","March","April","May","June","July","August","September","October",
    "November","December"};
string LastCompiled = __DATE__;
const char szAppName[] = "HPcursed";
const char InputPrompt[] = "Enter cmd or HELP : ";
int InputPromptLen;
WINPTRTYPE winleft,winrightupper,winrightlower;


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

/**********************************************************************/
PROCEDURE writestack() IS    // I don't think I need a Window param declaration

  ARRAYOF string strarray[StackSize];   // was ARRAY [1..8] OF STRTYP;
  string s,Xstr;
  int c,maxrow,maxcol;

  getmaxyx(winleft,maxrow,maxcol);
  GETSTACK(Stk);
  IF OutState EQ FIX THEN
    FOR c = 0;c < StackSize; ++c DO
      s = to_string(Stk[c]);
      s = CropNStr(s);
      IF Stk[c] > 10000 THEN
        s = AddCommas(s);
      ENDIF;
      strarray[c] = s;
//                                    LongStr.RealToFixed(stk[c],sigfig,strarray[c]);
    ENDFOR;
  ELSIF OutState EQ FLOAT THEN
    FOR c = 0;c < StackSize; ++c DO
      strarray[c] = to_string(Stk[c]);
//                                    LongStr.RealToEng(stk[c],sigfig,strarray[c]);
    ENDFOR;
  ELSE  // OutState EQ GEN
    FOR c = 0;c < StackSize; ++c DO
      strarray[c] = to_string(Stk[c]);
//                                    LongStr.RealToStr(stk[c],strarray[c]);
    ENDFOR;
  ENDFOR;

  wmove(winleft,1,0);
  wclrtoeol(winleft);
  wmove(winleft,1,0);
  wattron(winleft,A_BOLD);
  wprintw(winleft,"                X :");
//                                        wattron(winleft,COLOR_PAIR(aInvertedYellow));
  wattron(winleft,COLOR_PAIR(aYellow));
  wprintw(winleft,"%s\n",strarray[X].c_str());
  Xstr =  strarray[X];
  wattroff(winleft,COLOR_PAIR(aYellow));
  wattroff(winleft,A_BOLD);

  wattron(winleft,COLOR_PAIR(0));
  wclrtoeol(winleft);
  wprintw(winleft,"T1:%s\n",strarray[T1].c_str());

  wclrtoeol(winleft);
  wprintw(winleft,"T2:%s\n",strarray[T2].c_str());

  wclrtoeol(winleft);
  wprintw(winleft,"T3:%s\n",strarray[T3].c_str());

  wclrtoeol(winleft);
  wprintw(winleft,"T4:%s\n",strarray[T4].c_str());

  wclrtoeol(winleft);
  wprintw(winleft,"T5:%s\n",strarray[T5].c_str());

  wclrtoeol(winleft);
  wprintw(winleft,"Z :%s\n",strarray[Z].c_str());

  wclrtoeol(winleft);
  wattron(winleft,COLOR_PAIR(aBlue));
  wprintw(winleft,"Y :%s\n",strarray[Y].c_str());
  wattroff(winleft,COLOR_PAIR(aBlue)); 

  wclrtoeol(winleft);
  wattron(winleft,A_BOLD); 
  wattron(winleft,COLOR_PAIR(aCyan));
  wprintw(winleft,"X :%s\n",strarray[X].c_str());
  wattroff(winleft,COLOR_PAIR(aCyan)); 
  attroff(A_BOLD);

  wrefresh(winleft);
ENDPROC; // writestack

/****************************************************************/

PROCEDURE writehelp() IS
//           Now to use cooked mode here
  char ch;
  def_prog_mode();
  endwin();
  cout << " This is an RPN style calculator. " << endl;
  cout << "FIX,FLOAT,GEN -- output format.  SQRT,SQR,STO,RCL,ZEROREG,SHOREG" << endl;
  cout << "SWAP,SWAPXY,<>,><,~,` -- equivalent.  LASTX,@ -- Equivalent commands." << endl;
  cout << "ROLLDN -- X goes to T1.  Arrow keys -- Stack movement commands." << endl;
  cout << "EXP,LN,INT,ROUND,FRAC,MOD,HEX,PRIME,D2R,R2D,trig functions in deg." << endl;
  cout << "^ -- evaluate Y^X, put result in X.  ** -- abs Y, rounds X." << endl;
  cout << "JUL,GREG -- Z month, Y day, X year.  Pop stack as needed." << endl;
  cout << "TODAY -- Julian date of today's date." << endl;
  cout << "DOW -- Julian date number in X register." << endl;
  cout << "HCF -- Pushs highest common factor of Y and X onto stack." << endl;
  cout << "substitutions: = for +, ; for *, ' for +, ',' for -, _ is minus ." << endl;
  cout << "HOL -- holiday days and dates for year in X.  Press any key to continue." << endl;
  cin >> ch;
  reset_prog_mode();
  refresh();
ENDPROC; // writehelp
/*****************************************************************/
RETURN int FUNCTION GetRegIdx(char chr) IS
                                            /* Return 0..35  A = 10 and Z = 35 */
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

/*********************************************************************/
// ****************************************************************************************
PROCEDURE WriteReg() IS
  int i,maxrow,maxcol,row;
  bool FirstNonZeroFlag;
  string str;
  char ch;

  getmaxyx(winleft,maxrow,maxcol);
  row = maxrow/3;
//  IF (12 > maxrow) OR (50 > maxcol) THEN
//    wprintw(winleft," screen is too small.  Will exit now");
//    return;
//  ENDIF;
  wmove(winleft,row,0);                 // row,col
  wclrtobot(winleft);
  wmove(winleft,row,0);
  FirstNonZeroFlag  = false;
  str.clear();
  FOR i = 0; i <= 35; ++i DO
    IF Reg[i] EQ 0.0 THEN
      /* do nothing */
    ELSE
      IF NOT FirstNonZeroFlag THEN
        mvwprintw(winleft,row,0," The following registers are not zero.\n");
        FirstNonZeroFlag = true;
        ++row;
      ENDIF; // if FirstNonZeroFlag
      wprintw(winleft," Reg [");
      ch = GetRegChar(i);
      wprintw(winleft,"%c] = ",ch);
      str = to_string(Reg[i]);
      str = CropNStr(str);
      IF Reg[i] > 10000 THEN
        str = AddCommas(str);
      ENDIF;
      wprintw(winleft,"%s\n",str.c_str());
    END; /* if reg = 0 */
  END; /* for */
  wrefresh(winleft);
ENDPROC; // WriteReg


/*****************************************************************/

RETURN bool FUNCTION ODD(long i) IS
  return (i MOD 2) EQ 1;
ENDPROC;

/*********************************************************************/

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
/*++++*****************************************************************/

/********************************************************************/
RETURN string FUNCTION CleanRealString(string str) IS

  string s;
//  int i;
  int Sidx,STRidx,len;

//  upper := HIGH(str);
//  str[upper] := NULL;  (* Sentinal NULL for my algorithm *)
  len = str.length();
  Sidx = 0;
  STRidx = 0;
  WHILE (STRidx < len) DO
    IF ((str[STRidx] EQ '.') OR (CAP(str[STRidx]) EQ 'E') OR ((str[STRidx]>='0') AND (str[STRidx]<='9')) ) THEN
      s.at(Sidx) = str.at(STRidx);
      ++Sidx;
    ENDIF;
  ++STRidx;
  ENDWHILE;
  return s;
ENDPROC; // CleanRealString

/*++++*****************************************************************/

RETURN int FUNCTION ProcessInput() IS
/* Needs no params as all var's are global to module. */
/* the return int is a return code.  0 means normal return.  1 means exit normally or abnormally */
/* I need to work out a result area for things like hex out, prime, DOW, and perhaps others */
  int idx,c,I,maxrow,maxcol,ResultAreaRow;
  int NYD,July4,VetD,ChristmasD;
//  char ch1,ch2;
  bool ok;
  string str,substrng,s;
  double r;
  long LC;
//  char line[512];  not used anymore.

  writestack();
  getmaxyx(winleft,maxrow,maxcol);
  ResultAreaRow = maxrow - 10;
  inputprocessed = true;
  substrng = inputline.substr(0,3); // start position of 0, length of 3.
  IF inputline.length() EQ 0 THEN
    return 0;
  ELSIF inputline.compare("FIX") EQ 0 THEN
    OutState = FIX;
    sigfig = 4;  // for now, until I can figure out a better use for the sigfig variable.
  ELSIF inputline.compare("FLOAT") EQ 0 THEN
    OutState = FLOAT;
    sigfig = 4;  // until I can figure out a better use for the sigfig variable.
  ELSIF inputline.compare("GEN") EQ 0 THEN
    OutState = GEN;
    sigfig = 4;
  ELSIF inputline.compare("EXIT") EQ 0 THEN
    return 0;
  ELSIF substrng.compare("STO") EQ 0 THEN
    IF inputline.length() > 3 THEN            // remember that length is not same as idx of last character.
      idx = GetRegIdx(inputline.at(3));
    ELSE
      idx = 0;
    ENDIF;
    Reg[idx] = READX();
    RegAltered = true;
    WriteReg();
  ELSIF substrng.compare("RCL") EQ 0 THEN
    IF inputline.length() > 3 THEN
      idx = GetRegIdx(inputline.at(3));
    ELSE
      idx = 0;
    ENDIF;
    PUSHX(Reg[idx]);
  ELSIF inputline.compare("ZEROREG") EQ 0 THEN
    FOR c = 0; c <= 35; ++c DO
      Reg[c] = 0.0;
    ENDFOR; /* for c */
    RegAltered = true;
    WriteReg();
  ELSIF inputline.compare("SHOREG") EQ 0 THEN
    WriteReg();
  ELSIF inputline.compare("HEX") EQ 0 THEN
    r = round(READX());
    IF (r > 0.) AND (r <= 2.0E9) THEN
      str = ToHex(r);
      str += " hex";
      wmove(winleft,ResultAreaRow,0);
      wclrtobot(winleft);
      wmove(winleft,ResultAreaRow,0);
      wprintw(winleft," X = %s",str.c_str());
    ENDIF; // r is within range for the ToHex function.
  ELSIF inputline.compare("PRIME") EQ 0 THEN
    LC = round(READX());
    ok = IsPrime(LC);
    wmove(winleft,ResultAreaRow,0);
    wclrtobot(winleft);
    wmove(winleft,ResultAreaRow,0);
    IF ok THEN
      wprintw(winleft," %d is prime.",LC);
    ELSE
      wprintw(winleft," %d is not prime.",LC);
    ENDIF; // is prime
  ELSIF inputline.compare("DOW") EQ 0 THEN
    I = round(READX());
    str = DayNames[I MOD 7];
    wmove(winleft,ResultAreaRow,0);
    wclrtobot(winleft);
    wmove(winleft,ResultAreaRow,0);
    wprintw(winleft," X represents a %s",str.c_str());
  ELSIF inputline.compare("TOCLIP") EQ 0 THEN
    wmove(winleft,ResultAreaRow,0);
    wclrtobot(winleft);
    wmove(winleft,ResultAreaRow,0);
    wprintw(winleft," TOCLIP is not yet implemented.");
  ELSIF inputline.compare("FROMCLIP") EQ 0 THEN
    wmove(winleft,ResultAreaRow,0);
    wclrtobot(winleft);
    wmove(winleft,ResultAreaRow,0);
    wprintw(winleft," FROMCLIP is not yet implemented.");
  ELSIF inputline.compare("ABOUT") EQ 0 THEN
    wmove(winleft,ResultAreaRow,0);
    wclrtobot(winleft);
    wmove(winleft,ResultAreaRow,0);
    wprintw(winleft," Last compiled %s.",LastCompiled.c_str());
  ELSIF inputline.compare("HOL") EQ 0 THEN
    s = "HOL";
    GetResult(s);
    IF Holidays.valid THEN
      wmove(winleft,ResultAreaRow,0);
      wclrtobot(winleft);
      wprintw(winleft," Year : %d\n",Holidays.year);

      NYD = (JULIAN(1,1,Holidays.year) MOD 7);
      July4 = (JULIAN(7,4,Holidays.year) MOD 7);
      VetD = (JULIAN(11,11,Holidays.year) MOD 7);
      ChristmasD = (JULIAN(12,25,Holidays.year) MOD 7);
      wprintw(winleft,
        "New Years Day is a %s, MLK Day is January %d, ",
         DayNames[NYD],Holidays.MLK.d,Holidays.Pres.d);
      IF (maxcol-getcurx(winleft)) <= 25 THEN
        wprintw(winleft,"\n");
      ENDIF;
      wprintw(winleft, "Pres Day is February %d, ", Holidays.Pres.d);
      IF (maxcol-getcurx(winleft)) <= 25 THEN
        wprintw(winleft,"\n");
      ENDIF;
      wprintw(winleft, "Easter Sunday is %s %d, ", MonthNames[Holidays.Easter.m].c_str(),Holidays.Easter.d);
      IF (maxcol-getcurx(winleft)) <= 25 THEN
        wprintw(winleft,"\n");
      ENDIF;
      wprintw(winleft, "Mother's Day is May %d, ", Holidays.Mother.d);
      IF (maxcol-getcurx(winleft)) <= 25 THEN
        wprintw(winleft,"\n");
      ENDIF;
      wprintw(winleft,"Memorial Day is May %d, ", Holidays.Memorial.d);
      IF (maxcol-getcurx(winleft)) <= 25 THEN
        wprintw(winleft,"\n");
      ENDIF;
      wprintw(winleft,"Father's Day is June %d, ", Holidays.Father.d);
      IF (maxcol-getcurx(winleft)) <= 25 THEN
        wprintw(winleft,"\n");
      ENDIF;
      wprintw(winleft,"July 4 is a %s, ", DayNames[July4]);
      IF (maxcol-getcurx(winleft)) <= 25 THEN
        wprintw(winleft,"\n");
      ENDIF;
      wprintw(winleft,"Labor Day is Septempber %d, ",Holidays.Labor.d);
      IF (maxcol-getcurx(winleft)) <= 25 THEN
        wprintw(winleft,"\n");
      ENDIF;
      wprintw(winleft,"Columbus Day is October %d, ",Holidays.Columbus.d);
      IF (maxcol-getcurx(winleft)) <= 25 THEN
        wprintw(winleft,"\n");
      ENDIF;
      wprintw(winleft,"Election Day is November %d, ",Holidays.Election.d);
      IF (maxcol-getcurx(winleft)) <= 25 THEN
        wprintw(winleft,"\n");
      ENDIF;
      wprintw(winleft,"Veteran's Day is a %s ",DayNames[VetD]);
      IF (maxcol-getcurx(winleft)) <= 25 THEN
        wprintw(winleft,"\n");
      ENDIF;
      wprintw(winleft,"Thanksgiving is November %d, ",Holidays.Thanksgiving.d);
      IF (maxcol-getcurx(winleft)) <= 25 THEN
        wprintw(winleft,"\n");
      ENDIF;
      wprintw(winleft,"and Christmas Day is a %s.",DayNames[ChristmasD]);
      Holidays.valid = false;
      ENDIF; // Holidays.valid
    ELSIF inputline.compare("HELP") EQ 0 THEN
      writehelp();
      writestack();
      WriteReg();
    ELSE  /* Now must process the cmd, which is the whole reason for this pgm existing! */
      r = GetResult(inputline);
    ENDIF; // series of inputline.compare tests
    writestack();
    wrefresh(winleft);
    inputline.clear();
    return 0;
ENDPROC; // ProcessInput

// *****************************************************************************************************
//
RETURN int FUNCTION START() IS
  int InCh,maxrow,maxcol,col,midcol,childrow,rightmaxrow,rightmaxcol;
  string inputstr;

  getmaxyx(winleft,maxrow,maxcol);
  getmaxyx(winrightupper,rightmaxrow,rightmaxcol);
  childrow = 0;
  midcol = 0;
  
  writestack();
  WriteReg();

  WHILE true DO  // loop to read all characters until the routine encounters a return.
    IF inputprocessed THEN
      inputline.clear();
      wmove(winleft,0,0);
      wclrtoeol(winleft);
      mvwprintw(winleft,0,0,"%s",InputPrompt);
//      mvwprintw(winleft,1,0,"inputprocessed loop, left window.\n");
      wrefresh(winleft);
//      mvwprintw(winright,0,0,"inputprocessed loop, right window.\n");
//      wrefresh(winright);
      inputprocessed = false;
      wmove(winleft,0,InputPromptLen+1);
    ENDIF;
    InCh = wgetch(winleft);
    SWITCH InCh DO
      case KEY_BACKSPACE :
        IF inputline.length() > 0 THEN
          inputline.pop_back();
          col = getcurx(winleft) - 1;
          mvwprintw(winleft,0,col," ");
          wmove(winleft,0,col);
          wrefresh(winleft);
        ENDIF;
        break;
      case KEY_DC : // delete character key, IE DEL key
        IF inputline.length() > 0 THEN
          inputline.pop_back();
          col = getcurx(winleft) - 1;
          mvwprintw(winleft,0,col," ");
          wmove(winleft,0,col);
          wrefresh(winleft);
        ENDIF;
        break;
//      case KEY_DEL : // delete key duplicate, so I cannot use both this and KEY_DC
//        inputline.pop_back();
//        col = getcurx(stdscr) - 1;
//        mvwprintw(winleft,0,col," ");
//        wmove(winleft,0,col);
//        break;
      case KEY_ENTER:
        IF inputline.length() EQ 0 THEN
          return 0;
        ENDIF;
        wprintw(winrightupper,"%s\n",inputline.c_str());          // output inputline to something similar to the childwindow of the modula-2 version.
        wclrtoeol(winrightupper);
        wrefresh(winrightupper);
//        ++childrow;
//        IF childrow > rightmaxrow THEN
//          childrow = 0;
//        ENDIF;
        ProcessInput();
        break;
      case KEY_RETURN:
        IF inputline.length() EQ 0 THEN
          return 0;
        ENDIF;
        wprintw(winrightupper,"%s\n",inputline.c_str());          // output inputline to something similar to the childwindow of the modula-2 version.
        wclrtoeol(winrightupper);
        wrefresh(winrightupper);
//        ++childrow;
//        IF childrow > rightmaxrow THEN
//          childrow = 0;
//        ENDIF;
        ProcessInput();
        break;
      case ' ' :
        IF inputline.length() > 0 THEN  // ignore consecutive spaces
          wprintw(winrightupper,"%s\n",inputline.c_str()); 
          wclrtoeol(winrightupper);
          wrefresh(winrightupper);
//          ++childrow;
//          IF childrow > rightmaxrow THEN
//            childrow = 0;
//          ENDIF;
          ProcessInput();
        ENDIF;
        break;
      case '=' :
        inputline += '+';
        waddch(winleft,'+');
        break;
      case ';' :
        inputline += '*';
        waddch(winleft,'*');
        break;
      case '\'':
        inputline += '-';
        waddch(winleft,'-');
        break;
//      case '-':
//        inputline += CAP(InCh);
//        addch(InCh);
//        slen = inputline.length();  // has index of last character.
//        IF (slen > 0)   The old code had a line to specifically allow neg exponents.  I am not sure this is needed here.
      case '_':
        inputline += '-';
        waddch(winleft,'-');
        break;
      case KEY_F(1):
        writehelp();
        break;
      case KEY_F(2):
        writehelp();
        break;
      case KEY_PPAGE:  // PgUp key.  I used to do something.  But I found I never used it.  So now I'll do nothing.
        break;
      case KEY_NPAGE:  // PgDn key.  I used to do something.  But I found I never used it.  So now I'll do nothing.
        break;
      case KEY_IC:     // I think this is the insert key.  IC means insert character.
        break;
      case KEY_TAB:
        break;
      case KEY_LEFT:
        inputline += '~';  // send a swapxy command
        ProcessInput();
        break;
      case KEY_RIGHT:
        inputline += '~';  // send a swapxy command
        ProcessInput();
        break;
      case KEY_UP:
        IF inputline.length() > 0 THEN
          ProcessInput();
        ENDIF;
        PUSHX(READX());  // essentially a stack up operation
        writestack();
        break;
      case KEY_DOWN:
        inputline += '!';  // send a stack down command
        ProcessInput();
        break;
      case KEY_HOME:
        break;
      case KEY_END:
        ProcessInput();
        return 0;
        break;
      case KEY_F(10): // exit without processing whatever has already been typed.
        return 0;
        break;
      case KEY_ESC:  // exit without processing whatever has already been typed.
        return 0;
        break;
      default :
        inputline += CAP(InCh);
        waddch(winleft,InCh);
    ENDCASE;
    wrefresh(winleft);
    wrefresh(winrightupper);
  ENDWHILE;
ENDPROC; // START


//_______________________________________-------------------------------------______________________________________
//******************************************************************************************************************
int main(argcargv) IS
  double R;
  int I;
  string INBUF;
  bool StackFileExists,RegFileExists;
  const char StackFileName[] = "RPNStackFile.sav";
  const string StackFilenameString = StackFileName;
  const char RegFileName[] = "RPNRegFile.sav";
  char ch1;
  FileHandle RegFile,StackFile;
  int fullmaxrow,fullmaxcol,leftheight,leftwidth,leftstartrow,leftstartcol;
  int rightupperheight,rightlowerheight,rightwidth,rightstartrowupper,rightstartrowlower,rightstartcol;

  LastCompiled += " ";
  LastCompiled += __TIME__;

  initscr();
  cbreak();
  noecho();
  nonl();
  keypad(stdscr,TRUE);
  leaveok(stdscr,TRUE);
//  intrflush(stdscr,FALSE);
//  IF NOT has_colors() THEN
//    endwin();
//    printw("Your terminal does not support color\n");
//    exit(1);
//  ELSIF NOT can_change_color() THEN
//    printw("Your terminal cannot change colors\n");
//  ENDIF;

  start_color();
// This routine initializes the int COLORS and COLOR_PAIRS.
// COLORS is an int holding the # of defined colors.  COLOR_PAIRS is an int defining the # of defined color pairs.
// PROCEDURE ComposeAttribute(fore, back : Colors; style : FontStyleSet) : ScreenAttribute;
  init_pair(a,COLOR_BLACK,COLOR_WHITE);
//  aBold not needed as a separate color pair because it's an attribute called A_BOLD.
  init_pair(aInverted, COLOR_WHITE,COLOR_BLACK);
  init_pair(aInvertedRed,COLOR_RED,COLOR_BLACK);
  init_pair(aInvertedCyan,COLOR_CYAN,COLOR_BLACK);
  init_pair(aInvertedYellow,COLOR_YELLOW,COLOR_BLACK);
  init_pair(aInvertedBlue,COLOR_BLUE,COLOR_BLACK);
  init_pair(aRed,COLOR_BLACK,COLOR_RED);
  init_pair(aCyan,COLOR_BLACK,COLOR_CYAN);
  init_pair(aYellow,COLOR_BLACK,COLOR_YELLOW);
  init_pair(aBlue,COLOR_BLACK,COLOR_BLUE);

  InputPromptLen = strlen(InputPrompt);

  inputprocessed = true;
  RegAltered = false;

  StackFileExists = FileExists(StackFileName);
  IF StackFileExists THEN
    StackFile = fopen(StackFileName,"rb");
    FOR I=X; I<=T1; ++I DO
      fread(ADROF R, sizeof R, 1, StackFile);
      PUSHX(R);
    ENDFOR;
    fclose(StackFile);
  ENDIF; // StackFileExists

  RegFileExists = FileExists(RegFileName);
  IF RegFileExists THEN
    RegFile = fopen(RegFileName, "rb");
    FOR I=0; I < 36; ++I DO
      fread(ADROF R, sizeof R, 1, RegFile);
      Reg[I] = R;
    ENDFOR;
    fclose(RegFile);
  ENDIF; // RegFileExists
  FOR I = 0; I < 36; ++I DO
    IF abs(Reg[I]) < 1.0E-20 THEN
      Reg[I] = 0.0;
    ENDIF;
  ENDFOR;

  getmaxyx(stdscr,fullmaxrow,fullmaxcol);
  leftwidth = fullmaxcol/2;
  rightwidth = leftwidth;
  leftheight = fullmaxrow;
  rightlowerheight = 12;
  rightupperheight = fullmaxrow - rightlowerheight;
  leftstartrow = 0;
  leftstartcol = 0;
  rightstartrowupper = 0;
  rightstartcol = leftwidth + 1;
  rightstartrowlower = rightupperheight + 1;
  
  winleft = newwin(leftheight,leftwidth,leftstartrow,leftstartcol);

  winrightupper= newwin(rightupperheight,rightwidth,rightstartrowupper,rightstartcol);
  winrightlower= newwin(rightlowerheight,rightwidth,rightstartrowlower,rightstartcol);


  IF (winleft EQ nullptr) OR (winrightupper EQ nullptr) OR (winrightlower EQ nullptr) THEN
    endwin();
    cerr << "Could not create left or right windows.  Program exiting." << endl;
    return -1;
  ENDIF;

//  intrflush(winleft,FALSE);
  keypad(winleft,TRUE);

//  intrflush(winright,FALSE);
  keypad(winrightupper,TRUE);
  keypad(winrightlower,TRUE);
  scrollok(winrightupper,TRUE);
  scrollok(winrightlower,TRUE);

  IF argc > 1 THEN
    INBUF = getcommandline(argc,argv);
    GetResult(INBUF);
  ENDIF; // if command tail exists

  START();

  delwin(winleft);
  delwin(winrightupper);
  delwin(winrightlower);
  nl();
  nocbreak();
  echo();

  endwin();

  IF RegAltered THEN
    cout << " Should RegFile be written? ";
    cin >> ch1;
    IF CAP(ch1) EQ 'Y' THEN
      RegFile = fopen(RegFileName,"wb+");
      IF RegFile EQ nullptr THEN
        cerr << " Cannot open " << RegFileName << " for writing when closing this program." << endl;
        return -1;
      ENDIF;
      FOR I=0; I<36; ++I DO
        R = Reg[I];
        fwrite(ADROF R, sizeof R, 1, RegFile);
      ENDFOR;
/* Just to see if this could work.
      fwrite(Reg, sizeof Reg, 1, RegFile);
*/
    ENDIF; // if ch = Y
  ENDIF; // If RegAltered

  GETSTACK(Stk);
  StackFile = fopen(StackFileName,"wb+");
  IF StackFile EQ nullptr THEN
    cerr << " Cannot open " << StackFileName << " for writing upon closing program." << endl;
    return -1;
  ENDIF;
  FOR I=T1; I>=X; --I DO
    R = Stk[I];
    fwrite(ADROF R, sizeof R, 1, StackFile);
  ENDFOR;
  fclose(StackFile);

//  cout << "Done." << endl;  Used for debugging.  Not needed anymore.
  return 0;
ENDPROC; // main

