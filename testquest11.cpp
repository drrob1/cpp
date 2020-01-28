#include <stdio.h>
#include <iostream>
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
using namespace std;  // a compiler directive that prefixes each variable name, ie, std::myvariablename.  Since I rarely see it, I'm guessing it is optional.
void change(int x, int y, int &z) IS
        x = y + z;
        y = x + z;
        z = x + y;
        return;
END;


int main() IS
        int i = 2, j = 3;
        change(i, j, j);
        cout << i << j << endl;
        return 0;




END
