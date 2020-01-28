#include "macros.h"
#include "getcommandline.h"

// int main (int argc, char *argv[]) IS
int main (argcargv) IS
	string str;
        short int i=0;
        short int POINTERTO iPtr= ADROF i;
        int POINTERTO jPtr;
        char ch=' ';
	cout << "argc is " << argc << endl;
	for (int i=0; i < argc; ++i) DO
		cout << "argv[" << i << "] is " << argv[i] <<endl;
	ENDFOR;
	str = getcommandline(argc,argv);
	cout << "str from getcommandline is '" << str << "'" << endl;

        if (argv[argc] EQ 0) THEN
          cout << " argv sub argc is 0" << endl;
        ELSIF (argv[argc] EQ nullptr) THEN
          cout << " argv sub argc is nullptr" << endl;
        ENDIF;

//        jPtr = argv[argc];    error: cannot convert 'char*' to 'int*' in assignment
//
        cout << endl;
//        ch =  DEREF argv[argc];    gives a seg fault (core dumped)
        cout << "ch=" << ch;
//        i = int (*argv[argc]);     gives a seg fault (core dumped)
        cout << "i=" << i ;
        cout   << endl;

        cout << endl;
END



