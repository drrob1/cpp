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
#define REF &
#define VAR &
#define PROCEDURE void

#include <iostream>
using namespace std; 

#include <cstdio> 

#include <cstdlib>

#include <cstring>

#include <cctype>
#include <cmath>
#include <ctime>


/*
 * struct V3 IS
     double x,y,z;
   END

V3 sum(V3 const &a, V3 const &b) IS
  V3 tmp;
  tmp.x = a.x + b.x;
  tmp.y = a.y + b.y;
  tmp.z = a.z + b.z;
  return tmp;
END
*/



struct V3 IS
  private:

  public:
    double x,y,z;  // member data
    double length() IS return sqrt (x*x + y*y + z*z); END  // member fcn

    V3 sum(V3 const &b) IS
      V3 tmp;
      tmp.x = x + b.x;
      tmp.y = y + b.y;
      tmp.z = z + b.z;
      return tmp;
    END; // sum

    V3 SUM (V3 const REF b) IS   // just want to see if this will compile
      return V3(x+b.x,y+b.y,z+b.z);
    END;

    V3 scale(double const factor) IS  // member fcn, same as multiply by a constant factor
      V3 tmp;
      tmp.x = x * factor;
      tmp.y = y * factor;
      tmp.z = z * factor;
      return tmp;
    END; // scale, same as multiply by a constant factor

    PROCEDURE print () IS
      cout << "x: " << x << ", y: " << y << ", z: " << z << endl;
      return;
    END; // print

    V3 (double vx, double vy, double vz) IS
      x=vx; y=vy; z=vz; return;
    END; // V3 constructor function

    V3 () IS
      x=y=z=0.0;
      return;
    END; // another V3 constructior function.  More than 1 are allowed, but the param list must be different.

    V3 (const V3 REF src) IS
      x=src.x;
      y=src.y;
      z=src.z;
      return;
    END; // copy constructor

    V3 operator+ (const V3 REF b) const IS  // use of the const keyword here means that the receiving destination object cannot be changed.
      return V3(x+b.x, y+b.y, z+b.z);
    END; // operator+ overloading

    V3 operator* (const double factor) const IS  // note that this is defining a non-communitive scale multiplication that must be V3-type * factor which is a double-type.
      return V3(x*factor, y*factor, z*factor);
    END; // operator* overloading
END; // struct V3
/*
  V3 operator* (const double factor, const V3 REF b) IS  // the order of these params allows the expression, factor * b, to be evaluated correctly.
    return (b*factor); // this actually invokes the member function of the class V3, so in effect, it allows operator* to be reversible.
  END; // another operator* overloading to allow for the factor which is not of type V3 to be part of this
*/
int main () IS
  V3 p,q, * ptrP, * ptrQ;
  cout << " Enter a real number for the x coordinate only: ";
  cin >> p.x; // input x coord
  ptrP = ADR p;
  cout << ptrP->x <<endl;  // using the struct deref operator

  q = p.scale(0.5);    // p is the receiver object, and scale is a member fcn
  ptrQ = ADROF q;
  cout << ptrQ->length() <<endl;
END;








