/*
 *
 *  Created on: Oct 27, 2014
 *      Author: rob
 */
#include <cstring>
#include <cstdio>
#include <iostream>
using namespace std;
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <ctime>
#include <string>

#define EQ ==
#define NE !=
#define BEGIN {
#define END }
#define ENDIF }
#define ENDFOR }
#define ENDCASE }
#define ENDLOOP }
#define MOD %
#define AND &&
#define OR ||
#define NOT !
#define THEN {
#define ELSE }else{
#define ELSIF }else if{
#define IS {
#define DO {
#define LOOP {
#define ADROF &
#define ADR &
#define DEREF *
#define POINTERTO *
#define VAR &
#define REF &
#define PROCEDURE void
#define FUNCTION

typedef FILE* FileType;

FUNCTION string getcommandline(int argc, char *argv[]) IS
//		string s="";
		string s;
                int i;
		if (argc <= 1) THEN
		  return (s);
        ENDIF;
        s = argv[1];
        for (i=2; i < argc; ++i) DO
                s = s.append(" ");
 		        s = s.append(argv[i]);
 		ENDFOR;
 		return (s);
END;
/*
int main (int argc, char *argv[]) IS
	string str;
	cout << "argc is " << argc << endl;
	for (int i=0; i < argc; ++i) DO
		cout << "argv[" << i << "] is " << argv[i] <<endl;
	ENDFOR;
	str = getcommandline(argc,argv);
	cout << "str from getcommandline is " << str << endl;
END
*/


