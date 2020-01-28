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



int fun(int *b, int n){
    int count=0;
    count = 6 - n;
    if(n<=0)
       return 0;
    else if(b[count]%3==0)
       return b[count] + fun(b,n-1);

    else
       return b[count] - fun(b,n-1);
}
int main() {
    int a[] ={12, 11, 10, 9, 8, 7};
    cout<<fun(a, 6)<<endl;
    return 0;
}





