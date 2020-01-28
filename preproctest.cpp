#include "macros.h"

int main(int argc, char *argv[]) BEGIN
  int i=__cplusplus;
  unsigned int j=__cplusplus;
  printf(" value of __cplusplus in decimal is '%d', value in hex is '%x'\n",i,j);

  if (__cplusplus) THEN
    printf(" __cplusplus is true\n");
  ELSE
    printf(" __cplusplus is false");
  ENDIF;

  printf(" __STDC__ is '%d'\n",__STDC__);
//  cout << " value of __cplusplus is: " << __cplusplus;
//  cout << ",  value of __STDC__ is: " << __STDC__ << endl;



  return 0;
END
