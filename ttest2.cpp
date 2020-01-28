#include "/home/rob/mymacros.h"
#include "tokenizec.h"

int main(argcargv) IS
  TYPE TSNAMEType = ARRAY [0..3] OF STR10TYP;

  CONST TSNAME = TSNAMEType {'DELIM','OP','DGT','ALLELSE'};

  VAR INBUF,TOKEN : BUFTYP;
    TKNSTATE       : FSATYP;
    delimset,opset,dgtset : CHARSETTYP;
    RETCOD,C       : CARDINAL;
    I              : INTEGER;
    L              : LONGINT;
    R,r1,r2,r3,r4,r5,r6    : LONGREAL;
    CH             : CHAR;
    cid            : ChanId;
    flags          : FlagSet;
    openres        : OpenResults;


(* Overrides the defaults
  opset := CHARSETTYP{};
  NEWOPSET(opset);
(*  delimset := CHARSETTYP{' ',0C}; *)
  delimset := CHARSETTYP{};
  NEWDELIMSET(delimset);
  dgtset := CHARSETTYP{};
  NEWDGTSET(dgtset);
*)

  LOOP
    WriteString(' Input line : ');
    ReadString(INBUF.CHARS);
    SkipLine;
    WriteLn;
    TRIM(INBUF);
    IF STRCMPFNT(INBUF.CHARS,'QUIT') = 0 THEN EXIT; END(*IF*);
    IF INBUF.COUNT = 0 THEN EXIT; END(*IF*);
    INI1TKN(INBUF);
    LOOP
(*      GETTKNREAL(TOKEN,TKNSTATE,L,R,RETCOD); *)
      GETTKN(TOKEN,TKNSTATE,L,RETCOD); R := 0.;
(*      GETTKNSTR(TOKEN,I,RETCOD); *)
(*      GETTKNSTR(TOKEN,L,RETCOD); *)
      IF RETCOD > 0 THEN
        WriteString("GETTKN's RETCOD is ");
        WriteCard(RETCOD,0);
        WriteLn;
        EXIT;
      END(*IF*);
      WriteString(TOKEN.CHARS);

      WriteString(', TKNSTATE = ');
      WriteString(TSNAME[ORD(TKNSTATE)]);
(*
      WriteString(', I=');
      WriteInt(I,0);
*)
      WriteString(', L=');
      WriteLongInt(L,0);

      WriteString(', R=');
      WriteReal(R,15);
      WriteString(' fixed, float: ');
      WriteFixed(R,0,0);
      WriteString('   ');
      WriteFloat(R,10,0);


      WriteString(',  DELIMCH= ');
      WriteChar(DELIMCH);
      WriteString(', DELIMSTATE= ');
      WriteString(TSNAME[ORD(DELIMSTATE)]);
      WriteLn;
      WriteString(' Call UNGETTKN? ');
      ReadChar(CH);
      SkipLine;
(*      WriteChar(CH); *)
      WriteLn;
      IF CAP(CH) = 'Y' THEN UNGETTKN(RETCOD); END(*IF*);
      IF RETCOD > 0 THEN
        WriteString(' Nonzero RETCOD from UNGETTKN.');
        WriteLn;
      END(*IF*);
    END(*LOOP*);
  END(*LOOP*);
END TTEST2.
