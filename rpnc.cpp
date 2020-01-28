// (C) 1990-2013.  Robert W. Solomon.  All rights reserved.
#include "macros.h"
#include "hpcalcc.h"
#include "getcommandline.h"
#include "tokenizec.h"
#include "timlibc.h"
#include "holidaycalc.h"

extern  HolType Holidays;

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


int main (argcargv) IS
/*
  This module uses the HPCALC module to simulate an RPN type calculator.
  REVISION HISTORY
  ----------------
   1 Dec 89 -- Changed prompt.
- 24 Dec 91 -- Converted to M-2 V 4.00.  Changed params to GETRESULT.
  25 Jul 93 -- Output result without trailing insignificant zeros,
                imported UL2, and changed prompt again.
   3 Mar 96 -- Fixed bug in string display if real2str fails because
                number is too large (ie, Avogadro's Number).
  18 May 03 -- First Win32 version.  And changed name.
   1 Apr 13 -- Back to console mode pgm that will read from the cmdline.  Intended to be a quick and useful little utility.
                And will save/restore the stack to/from a file.
   2 May 13 -- Will use console mode flag for HPCALC, so it will write to console instead of the terminal module routines.
                And I now have the skipline included in MiscStdInOut so it is removed from here.
  15 Oct 13 -- Now writing for gm2 under linux.
  22 Jul 14 -- Converting to Ada.
   6 Dec 14 -- Converting to cpp.
  20 Dec 14 -- Added macros for date and time last compiled.
  31 Dec 14 -- Started coding HOL command.
   1 Jan 15 -- After getting HOL command to work, I did more fiddling to further understand c-strings and c++ string class.
  10 Jan 15 -- Playing with string conversions and number formatting.
   5 Nov 15 -- Added the RECIP, CURT, VOL commands to hpcalc.cpp
  22 Nov 15 -- Noticed that T1 and T2 stack operations are not correct.  This effects HP2cursed and rpnc.
  13 Apr 16 -- Adding undo and redo commands, which operate on the entire stack not just X register.
   2 Jul 16 -- Fixed help to include PI command, and changed pivot for JUL command.  See hpcalcc.cpp
   7 Jul 16 -- Added UP command to hpcalcc.cpp
   8 Jul 16 -- Added display of stack dump to always happen, and a start up message.
*/

  double R;
  int I,Y,NYD,July4,VetD,ChristmasD;
  string INBUF,ans;
  bool StackFileExists;
  double Stk[StackSize];
  FileHandle StackFile;
  CONSTARRAYOFCHAR StackFileName = "RPNStack.sav";
  const string STACKFILENAMEstring = StackFileName;
  CONSTARRAYOFCHAR DAYNAMES[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//  CONSTARRAYOFCHAR MONTHNAMES[] = {"filler","January","February","March","April","May","June","July","August","September","October",
  string MONTHNAMES[13] = {"filler","January","February","March","April","May","June","July","August","September","October",
    "November","December"};
  char line[256];
//                      string LastCompiledDate = __DATE__;   string LastCompiledTime = __TIME__;   string LastCompiled = LastCompiledDate;
  string LastCompiled = __DATE__;
  LastCompiled += " ";
  LastCompiled += __TIME__;

// There is no reg file yet to process.


  StackFileExists = FileExists(StackFileName);
  IF StackFileExists THEN
    StackFile = fopen(StackFileName,"rb");
    FOR I=X; I<=T1; ++I DO
      fread(ADROF R, sizeof R, 1, StackFile);
      PUSHX(R);
    ENDFOR;
    fclose(StackFile);
  ENDIF; // stackfileexists

  PushStacks();

  cout << " HP RPN type calculator started.  Last compiled "  << LastCompiled << endl << endl;

  IF argc > 1 THEN
    INBUF = getcommandline(argc,argv);
  ELSE
    cout <<" Enter calculation, HELP or Enter to exit: ";
    getline(cin,INBUF);
  ENDIF; // if command tail exists

  PushStacks();

  WHILE INBUF.length() > 0 DO
    R = GetResult(INBUF);
    ans = to_string(R);
    ans = CropNStr(ans);
    IF R > 10000 THEN
      ans = AddCommas(ans);
    ENDIF;
    cout << endl;
    cout << endl;
    cout << " Result = ";
    OutputFixedOrFloat(R);
    cout << "             " << ans;
    cout << endl;
    cout << endl;
    IF INBUF.compare("about") EQ 0 THEN
      cout << endl << " Last compiled " << LastCompiled << endl << endl << endl;
//      cout << endl  << " last compiled " <<__DATE__ <<" " <<__TIME__ <<endl << endl << endl;
    ELSIF Holidays.valid THEN
      cout << " For year ";
      Y = Holidays.year;
      cout << Y << endl;
      NYD = (JULIAN(1,1,Y) MOD 7);
      sprintf(line,"New Years Day is a %s, MLK Day is January %d, Pres Day is February %d, Easter Sunday is %s %d, Mother's Day is May %d",
        DAYNAMES[NYD],Holidays.MLK.d,Holidays.Pres.d,MONTHNAMES[Holidays.Easter.m].c_str(),Holidays.Easter.d,Holidays.Mother.d);
      cout << line << endl;

      July4 = (JULIAN(7,4,Y) MOD 7);
      sprintf(line,"Memorial Day is May %d, Father's Day is June %d, July 4 is a %s, Labor Day is Septempber %d, Columbus Day is October %d",
        Holidays.Memorial.d,Holidays.Father.d,DAYNAMES[July4],Holidays.Labor.d,Holidays.Columbus.d);
      cout << line << endl;

      VetD = (JULIAN(11,11,Y) MOD 7);
      ChristmasD = (JULIAN(12,25,Y) MOD 7);
      sprintf(line,"Election Day is November %d, Veteran's Day is a %s, Thanksgiving is November %d, and Christmas Day is a %s.",
        Holidays.Election.d,DAYNAMES[VetD],Holidays.Thanksgiving.d,DAYNAMES[ChristmasD]);
      cout << line << endl;
      cout << endl << endl << endl;
      Holidays.valid = false;
    ENDIF;
    cout << endl << endl << endl;
    GetResult("DUMP");   // discard result.  Only need stack dump executed.
    cout << endl << endl << endl;
    cout << " Enter calculation, HELP or Enter to exit: ";
    getline(cin,INBUF);
    cout << endl;
  ENDWHILE;
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
END; // main in rpnc
