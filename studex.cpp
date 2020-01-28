#include "/home/rob/mymacros.h"

class Student IS
  protected:
  public:
    static int noOfStudents;
    string name;

    Student(PointerToConstCharType pName = "no name") : name(pName) IS
      noOfStudents++;
    END; // constructor

    ~Student() IS
      noOfStudents--;
    END; // destructor

    const string REF getName() IS return name; END;
    static FUNCTION int getNumber() IS return noOfStudents; END;
END; // class Student

int Student::noOfStudents=0;  // allocates space for the static data member and init's to 0.

FUNCTION int main(argcargv) IS

  Student s1("Chester");
  Student POINTER pS2 = new Student("Charlie");

  printf("No of s1.students is %d, No of Students::students is %d.  \n", s1.noOfStudents,
                                                                            Student::noOfStudents);

  cout << "Created " << s1.getName() << " and " << pS2->getName() << endl;

  cout << "Deleting " << pS2->getName() << endl;
  delete pS2;
  cout << " Number of students is now " << Student::getNumber() << endl;

  return 0;
END;

