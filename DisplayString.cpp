/*
 * DisplayString.cpp
 *
 *  Created on: Nov 3, 2014
 *      Author: rob
 */
#include "/home/rob/mymacros.h"

// From p 140 of c++ for dummies (c) 2014, Stephen Davis.

FUNCTION int main(argcargv) IS
  PointerToConstCharType szString="My Name";
  cout << "The const string array is '" << szString << "'" << endl;

  // array as an array
  cout << " Display the string as an array: ";
  for (int i=0; i < 7; ++i) DO
		  cout << szString[i];
  ENDFOR;
  cout << endl;

  // now using typical pointer arith idiom
  cout << " Display string using a pointer: ";
  PointerToConstCharType pszString=szString;
  // const CharPointerType applies the const to the pointer itself and not to the values.  That's why it did not work until I had a macro
  // for const char * so that the const part refers to the contents and not the pointer itself.  Which is what is supposed to happen.


  while (DEREF pszString) DO
		  cout << DEREF pszString++;
  ENDWHILE;
  cout << endl;

  return 0;
END;



