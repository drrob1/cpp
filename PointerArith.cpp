/*
 * PointerArith.cpp
 *
 *  Created on: Nov 2, 2014
 *      Author: rob
 */
#include "macros.h"

// Pages 138 ff from Chap 9 of Dummies C++ (c) 2014

PROCEDURE displayArray(int intArray[],int nSize) IS
  cout << "The value of the array is: " << endl;
  int POINTERTO pArray = intArray;  // init the pointer array with the address of the array
  for (int n=0; n < nSize; n++, pArray++) LOOP
		  cout << n << ": " << DEREF pArray << ", value of pArray is " << pArray   << endl;
  ENDLOOP;
  cout << endl;
END;

FUNCTION int main(int argc, char POINTERTO argv[]) IS
		int array[] = {4,3,2,1};
        displayArray(array, 4);
/*
        cout << "Press enter to continue" << endl;
        cin.ignore(10,'\n');
        cin.get();
*/
        return 0;
END;



