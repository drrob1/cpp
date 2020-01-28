#include "macros.h"
#include "tokenizec.h"
#include "getcommandline.h"

int main(argcargv) IS
  const string TSNAME[4] = {"DELIM","OP","DGT","ALLELSE"};

  string INBUF;
  char line[200];
  TokenType token;
//  int i,c;
  char CH;
  bool EOL,STOP=false;

  WHILE NOT STOP DO
    IF argc > 1 THEN
      INBUF = getcommandline(argc,argv);
      STOP = true;
    ELSE
      cout << " Input line : " ;
      getline(cin,INBUF);
    ENDIF;
    IF INBUF.compare("QUIT") EQ 0 THEN goto EXIT; ENDIF;
    IF INBUF.length() EQ 0 THEN goto EXIT; ENDIF;
    cout << "INBUF contains '" << INBUF << "', length =" << INBUF.length() << endl;
    INITKN(INBUF);
    INBUF.clear();
    
//    do {
    REPEAT
//      EOL = GETTKN(token); 
      EOL = GETTKNREAL(token); 
//      EOL = GETTKNSTR(token); 
//      EOL = GETTKNEOL(token); 
      cout << "Token = '";
      cout << token.uStr;

      cout << "', TKNSTATE = ";
      cout << TSNAME[token.STATE];

      cout << ", iSum= " << token.iSum;
      cout << ", rSum= " << token.rSum;

      cout << ",  DELIMCH= " << token.delimCH;
      cout << ", DelimCH = :" << (int) token.delimCH;
      cout << ", DELIMSTATE= " << TSNAME[token.DelimState];
      cout << ", EOL = " << EOL;
      cout << endl;
      IF NOT EOL THEN
        cout << " Call UNGETTKN? ";
        cin.getline(line,200);
//        cin.get(CH);  This line gave odd I/O behavior in that it would swallow a \n.  
//        Don't mix getline and get for this reason.
        cout << endl;
        CH = Cap(line[0]);
        IF CH EQ 'Y' THEN UNGETTKN(); ENDIF;
      ENDIF;
    UNTIL(EOL);
//    UNTILL(EOL);
//    } while (NOT EOL);
  ENDWHILE;

  EXIT:

  return 0;
END
