#include "/home/rob/mymacros.h"

class Person IS
  protected:
    string POINTER pName;

  public:
    Person(const char POINTER pN) IS
      pName = new string(pN);
      cout << "Constucting " << *pName << endl;
    END; // constructor

    Person(Person COPY p) IS
      cout << "Copying " << DEREF p.pName << endl;
      pName = new string("Copy of ");
      DEREF pName += DEREF p.pName;
    END; // copy constructor

    Person(Person MOVE p) IS
      cout << "Moving " << DEREF p.pName << endl;
      pName = p.pName;  // assign pointer from source object to destination object
      p.pName = nullptr; // zeroing out this pointer so that the destructor does not delete the memory being used.
    END; // move consttructor

    ~Person() IS
      if (pName NE nullptr) THEN
        cout << "Destructing " << DEREF pName << endl;
        delete pName;
      ELSE
        cout << "Destructing null object" << endl;
      ENDIF;
    END; // destructor
END; // class person

FUNCTION Person fn2(Person p) IS
  cout << "Entering fn2" << endl;
  return p;
END;

FUNCTION Person fn1(CharPointerType pName) IS
  cout << "Entering fn1_" << endl;
  return fn2(DEREF new Person(pName));
END;

int main() IS
  Person s(fn1("Scruffy"));  // constructor call
  return 0;
END;
  
