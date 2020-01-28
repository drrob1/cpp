#include "/home/rob/mymacros.h"
#include "timlibc.h"

char FUNCTION cap(char c) IS  // h because it is defined in a .h file
  return toupper(c);
END; // hcap

int main(argcargv) IS
  int i,j,k;
  int M,D,Y;
  unsigned int jd;
  string str,s,s0,s1,s2,s3,s4;

  DateTimeType DT;

  TIME2MDY(M,D,Y);
  cout << "M=" << M << ", D=" << D << ", Y=" << Y << endl;
  str = MDY2STR(M,D,Y,s);
  cout << "MDY2STR: str = " << str << ", s= " << s;
  MDY2STR(M,D,Y,s0);
  cout << ", MDY2STR s0 = " << s0 << endl;

  DT = GetDateTime();
  cout << "GetDateTime: time is " << DT.hours << ":" << DT.minutes << ":" << DT.seconds << endl;
  cout <<"TimeInfo (month number starts at 0) " << DT.TimeInfo.tm_mon << "-" << DT.TimeInfo.tm_mday << "-" << DT.TimeInfo.tm_year <<endl;
  cout <<"TimeInfo " << DT.TimeInfo.tm_hour <<":" << DT.TimeInfo.tm_min << ":" << DT.TimeInfo.tm_sec <<endl;

  cout << " rawtime " << DT.rawtime <<endl;

  jd = JULIAN(M,D,Y);
  GREGORIAN(jd,i,j,k);
  cout << " Julian= " << jd <<", Gregorian: " << i << "-" << j << "-" << k << endl;
  cout << " Testing CAP of c : " << 'c' <<  CAP('c') << endl;
  cout << " testing cap of d : " << 'd' <<  cap('d') << endl;

  return 0;
END;
