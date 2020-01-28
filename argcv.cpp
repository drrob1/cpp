#include "macros.h"
#include "getcommandline.h"

// int main (int argc, char *argv[]) IS
int main (argcargv) IS
        int si=0;
        int POINTERTO iPtr= ADROF si;
	int j;
        int POINTERTO jPtr= ADROF j;
        char ch;
	cout << "argc is " << argc << endl;

        if (argv[argc] EQ 0) THEN
          cout << " argv sub argc is 0" << endl;
        ENDIF;
        if (argv[argc] EQ nullptr) THEN
          cout << " argv sub argc is nullptr" << endl;
        ENDIF;

//        jPtr = argv[argc];    error: cannot convert 'char*' to 'int*' in assignment
//
//        iPtr = ADROF argv[argc];   cannot convert 'char**' to 'short int*' in assignment
//	cout << " attempt to convert nullptr to an int." << DEREF iPtr << endl;

        cout << endl;
        ch =   argv[argc][0]; //   gives a seg fault (core dumped)
        cout << "ch=" << ch;
        si = int (argv[argc][0]);  //   gives a seg fault (core dumped)
        cout << ", si=" << si ;
        cout   << endl;

	j = atoi(argv[argc]);
	cout << " Use of atoi to convert nullptr. j="  << j << endl;


        cout << endl;
END



