#include "macros.h"

/*
  From cplusplus.com

 getenv example: getting path 
// printf lives in stdio.h or cstdio
// getenv lives in stdlib.h or cstdlib

  char* pPath;
  pPath = getenv ("PATH");
  if (pPath!=NULL)
    printf ("The current path is: %s",pPath);
  return 0;

*/


int main () IS
  char PtrType HomeDir;
  char PtrType UsersName;
  string StackFileString, RegFileString, s1,s2;
  const char StackFileName[] = "RPNStackFile.sav";
  const char RegFileName[] = "RPNRegFile.sav";
  ARRAYOF char StackFileFullName[200], ARRAYOF RegFileFullName[200];
//   ARRAYOF char RegFileFullName[200];   Just to see if it is allowed to define both on 1 line.

  HomeDir = getenv("HOME");
  IF HomeDir NE NULL THEN
//                           printf(" The home dir is: %s\n",HomeDir);  This line works.
    StackFileString = HomeDir; // If I deref, then only the first char gets copied.
    RegFileString = HomeDir;

    StackFileString += "/";
    StackFileString += StackFileName;
    RegFileString += "/";
    RegFileString += RegFileName;
    cout << " StackFileString: " << StackFileString << ",   RegFileString: " << RegFileString << endl;

    strcpy(StackFileFullName,HomeDir);
    strcat(StackFileFullName,"/");
    strcat(StackFileFullName,StackFileName);
    s1 = StackFileFullName;

    strcpy(RegFileFullName,HomeDir);
    strcat(RegFileFullName,"/");
    strcat(RegFileFullName,RegFileName);
    s2 = RegFileFullName;
//                                 cout << " StackFileFullName: " << s1 << ",  RegFileFullName: " << s2 << endl;
    cout << " StackFileFullName: " << StackFileFullName << ",  RegFileFullName: " << RegFileFullName << endl;
  ENDIF;

  UsersName = getenv("USER");
  IF UsersName NE NULL THEN
//                           printf(" The User is: %s\n",UsersName);   This line works.
    cout << " User is: " << UsersName << endl;  // Deref only outputs the first char.
    cout << " User is: " << UsersName << endl;
  ENDIF;

END; // main

/* strcat example */
//                char * strcat ( char * destination, const char * source ); 
//  char str[80];
//  strcpy (str,"these ");
//  strcat (str,"strings ");
//  strcat (str,"are ");
//  strcat (str,"concatenated.");
//  puts (str);


// And the c_str() member function of the string class is what returns a c-string.  Assigning a c-string
// is automatically handled by the assignment member function.
