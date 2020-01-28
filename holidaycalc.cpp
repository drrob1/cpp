#include "macros.h"
#include "holidaycalc.h"
#include "timlibc.h"
/*
  REVISION HISTORY
  ----------------
  5 Apr 88 -- 1) Converted to M2 V3.03.
              2) Imported the REALLIB and CALLIB modules, and deleted their
                  code from here.  They were originally created here, but
                  it seems more appropriate to leave them in a central
                  module and import them.
  19 Mar 89 -- 1) Imported the newly created HPCALC to allow date arithmetic.
                  The rest of the code was modified to take advantage of this
                  new capability.
               2) Fixed a bug in the error reporting from the EASTER proc
                  so that a FOR index variable does not get assigned 0.
  30 Mar 89 -- 1) Fixed bug in the PR and HOL cmds that ignored 2 digit
                  years
               2) Added reminder to GREG help line to use quotes to force
                  the date to be taken as an ALLELSE TKNSTATE.
  26 Dec 90 -- 1) Utilized the GETTKNSTR procedure where appropriate.
               2) UL2 is used instead of UTILLIB.
               3) Added GETTKNEOL proc to deal with GREG & DOW cmds.
  25 Jul 93 -- 1) Dropping requirement of CALCCMD by passing cmdline thru
                   to GETRESULT.
               2) Allowed empty command line to quit after confirmation.
               3) Eliminated writing of trailing insignificant 0's from
                   arithmetic functions.
               4) Imported TKNRTNS instead of TOKENIZE.
               5) Eliminated need for GETTKNSTR by improving algorithm.
               6) Deleted GETTKNEOL proc as it is no longer used.  If
                   needed, it may be imported from TKNRTNS now.
  18 May 03 -- Conversion to Win32 using Stony Brook Modula-2 V 4
  25 Dec 14 -- Converted to a module to get holiday dates by the calculator, using HolMod written for the Cal program.
   1 Jan 15 -- Converting to cpp, and combining HolMod into this module.
*/

//******************************** SUBTDAYS ********************************

RETURN int FUNCTION SUBTDAYS(int C, int Y) IS
/*
Subtract Days.
Computes how many days to subtract from the holiday depending on the year.
Days to Subtract = C + [5/4 Y] - [3/4 (1 + [Y/100])  ]) MOD 7
*/
  return ((C + (5*Y/4) - 3*(1 + (Y/100)) / 4) MOD 7);
ENDFUNC;// SUBTDAYS

/*****************************************************************************************/
PROCEDURE CalcMLK(int year, int REF day) IS
/*
 Find the date of MLK day by finding which day back from Jan 21 is a Monday.
*/

unsigned int J;

  day = 21;
  J = JULIAN(1,day,year);
  WHILE (J MOD 7) NE 1 DO
    --day;
    J = JULIAN(1,day,year);
  ENDWHILE;
ENDPROC;// CalcMLK

//************************************ EASTER ******************************
PROCEDURE EASTER(int YEAR, int REF MM, int REF DD) IS
/*
EASTER.
This routine computes the golden number for that year, then Easter Sunday is
the first Sunday following this date.  If the date is a Sunday, then Easter
is the following Sunday.
*/
int GOLDENNUM,IGNORE;
unsigned int JULDATE;

  IF (YEAR < 1900) OR (YEAR > 2500) THEN
    MM = 0;
    DD = 0;
  ELSE
    GOLDENNUM = (YEAR MOD 19) + 1;
    SWITCH GOLDENNUM DO
      case  1: // APR 14
        MM = 4;
        DD = 14;
        break;
      case  2: // APR 3
        MM = 4;
        DD = 3;
        break;
      case  3: // MAR 23
        MM = 3;
        DD = 23;
        break;
      case  4: // APR 11
        MM = 4;
        DD = 11;
        break;
      case  5: // MAR 31
        MM = 3;
        DD = 31;
        break;
      case  6: // APR 18
        MM = 4;
        DD = 18;
        break;
      case  7: // APR 8
        MM = 4;
        DD = 8;
        break;
      case  8: // MAR 28
        MM = 3;
        DD = 28;
        break;
      case  9: // APR 16
        MM = 4;
        DD = 16;
        break;
      case 10: // APR 5
        MM = 4;
        DD = 5;
        break;
      case 11: // MAR 25
        MM = 3;
        DD = 25;
        break;
      case 12: // APR 13
        MM = 4;
        DD = 13;
        break;
      case 13: // APR 2
        MM = 4;
        DD = 2;
        break;
      case 14: // MAR 22
        MM = 3;
        DD = 22;
        break;
      case 15: // APR 10
        MM = 4;
        DD = 10;
        break;
      case 16: // MAR 30
        MM = 3;
        DD = 30;
        break;
      case 17: // APR 17
        MM = 4;
        DD = 17;
        break;
      case 18: // APR 7
        MM = 4;
        DD = 7;
        break;
      case 19: // MAR 27
        MM = 3;
        DD = 27;
    ENDCASE;
  ENDIF;
/*
  Now find next Sunday.
*/
  JULDATE = JULIAN(MM,DD,YEAR);
  WHILE (JULDATE MOD 7) NE 0 DO
    ++JULDATE;
  ENDWHILE;
  GREGORIAN(JULDATE,MM,DD,IGNORE);
ENDPROC; // EASTER

//*****************************************************************************************

PROCEDURE GetHolidays(HolType REF Holidays) IS

  Holidays.MLK.m = 1;
  CalcMLK(Holidays.year,Holidays.MLK.d);

  Holidays.Pres.m = 2;
  Holidays.Pres.d = 21 - SUBTDAYS(2,Holidays.year-1);

  EASTER(Holidays.year,Holidays.Easter.m,Holidays.Easter.d);

  Holidays.Mother.m = 5;
  Holidays.Mother.d =  14 - SUBTDAYS(0,Holidays.year);

  Holidays.Memorial.m = 5;
  Holidays.Memorial.d = 31 - SUBTDAYS(2,Holidays.year);

  Holidays.Father.m = 6;
  Holidays.Father.d = 21 - SUBTDAYS(3,Holidays.year);

  Holidays.Labor.m = 9;
  Holidays.Labor.d = 7 - SUBTDAYS(3,Holidays.year);

  Holidays.Columbus.m = 10;
  Holidays.Columbus.d = 14 - SUBTDAYS(5,Holidays.year);

  Holidays.Election.m = 11;
  Holidays.Election.d = 8 - SUBTDAYS(1,Holidays.year);

  Holidays.Thanksgiving.m = 11;
  Holidays.Thanksgiving.d = 28 - SUBTDAYS(5,Holidays.year);

ENDPROC; // GetHolidays

// END holidaycalc.cpp
