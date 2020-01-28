#include "macros.h"
#include "getcommandline.h"

// int main (int argc, char *argv[]) IS
int main (argcargv) IS
	string str;
        short int si;
        short int POINTER iPtr= ADROF si;
	int j,len;
        int POINTER jPtr= ADROF j;
        char POINTER ch='\0';   // looking at this now, I actually made ch a c-string.
        char POINTER s;
	cout << "argc is " << argc << endl;
	for (int i=0; i < argc; ++i) DO
		cout << "argv[" << i << "] = " << argv[i] <<endl;
	ENDFOR;
	str = getcommandline(argc,argv);
	cout << "str from getcommandline is '" << str << "'" << endl;

        if (argv[argc] EQ 0) THEN
          cout << " argv sub argc is 0" << endl;
        END;
        if (argv[argc] EQ nullptr) THEN
          cout << " argv sub argc is nullptr" << endl;
        ENDIF;

//        jPtr = argv[argc];    error: cannot convert 'char*' to 'int*' in assignment
//
//        iPtr = ADROF argv[argc];   cannot convert 'char**' to 'short int*' in assignment
//	cout << " attempt to convert nullptr to an int." << DEREF iPtr << endl;

        cout << endl;
//        ch =   argv[argc]; //   gives a seg fault (core dumped)
//        cout << "ch=" << ch;
        si = intptr_t (argv[argc]); 
        j = (intptr_t) argv[argc]; 
        cout << " si=" << si << ", j=" << j;
        cout   << endl;

//	j = atoi(argv[argc]);
//	cout << " Use of atoi to convert nullptr. j="  << j << endl;

        s = argv[1];
        len = strlen(s);
        cout << " argv sub 1 contents are : ";
        for (int i=0; i<= len; ++i) DO
//          j = int (s[i]);  // not standard syntax, but works
          j = (int) s[i];    // seems that more correct syntax is this line
          cout << s[i] <<", '" << j << "'  ," ;
        ENDFOR;


        cout << endl;
END



