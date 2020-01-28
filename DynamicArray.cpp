#include "macros.h"

class DynamicArrayType IS
  protected:
    PROCEDURE copyDArray(const DynamicArrayType REF da);
    PROCEDURE DestroyDArray();

    int nLength;
    int POINTER pArray;  // I think I could declare this as int pArray[]; and it would have the same meaning.

  public:
    DynamicArrayType(int nLengthOfArray = 0) : nLength(nLengthOfArray), pArray(nullptr) IS   // define constructor
      cout << "Creating DynamicArrayType of length =" << nLength << endl;
      if (nLength > 0) THEN
        pArray = new int[nLength];
      ENDIF;
    END; // constructor

    DynamicArrayType(DynamicArrayType REF da) IS  // define copy constructor
      cout << "Copying DArray of length = " << da.nLength << endl;
      copyDArray(da);
    END; // copy constructor

    ~DynamicArrayType() IS // define destructor
      DestroyDArray();
    END; // destructor
    
    DynamicArrayType REF operator=(const DynamicArrayType REF src) IS
      cout << "Assigning source of length = " << src.nLength << " to target of length = " << this->nLength << endl;

      DestroyDArray(); // destroy existing contents of DynamicArrayType object
      copyDArray(src); // replaced with new object
      return POINTERTO this;
    END; // assignment operator overloading function

    int REF operator[] (int index) IS  // overload [] operator
      return pArray[index];
    END; // overload [] operator

    FUNCTION int size() IS
      return nLength;
    END;

    PROCEDURE display(ostream REF out) IS
      if (nLength > 0) THEN
        out << pArray[0];
        for (int i=1; i < nLength; ++i) DO
          out << ", " << pArray[i];
        ENDFOR;
      ENDIF;
    END; // display procedure
END; // class DynamicArrayType, which the book calls DArray.

PROCEDURE DynamicArrayType::copyDArray(const DynamicArrayType REF source) IS
  nLength = source.nLength;
  pArray = nullptr;
  if (nLength > 0) THEN
    pArray = new int[nLength];
    for(int i=0; i < nLength; ++i) DO
      pArray[i] = source.pArray[i];
    ENDFOR;
  ENDIF;
END; // procedure copyDArray

PROCEDURE DynamicArrayType::DestroyDArray() IS
  nLength = 0;
  delete pArray;
  pArray = nullptr;
END; // DestroyDArray

FUNCTION int main(argcargv) IS
  DynamicArrayType da1(5);
  for (int i=0; i < da1.size(); ++i) DO
    da1[i] = i;  // this line uses the user defined index operator to access the members of the array.
  ENDFOR;
 
  cout << "da1 =";
  da1.display(cout);
  cout << endl;
          
  DynamicArrayType da2=da1;
  da2[2] = 20;  // change a value in the copy
  cout << "da2=";
  da2.display(cout);
  cout<<endl;

  da2 = da1; // this overwrites da2 with the original da1
  cout << " da2=";
  da2.display(cout);
  cout << endl;

  return 0;
END;

