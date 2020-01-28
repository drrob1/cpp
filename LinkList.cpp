#include "macros.h"


/*
  REVISION HISTORY
  ----------------
  3/12/15 From modula2.org tutorial.  This section deals with dynamic memory usage.  I will change it around so I better understand it.
              And I've made it a double linked list.
  3/13/15 Will add output of the pointers so I can compare this with the prev and next field contents.  And I changed the name
              of variable AnEntry to AnEntryPointer.
  3/15/15 Converting to C++, and changing more names to be clearer that they are pointers.
  3/18/15 Removed the char cast to see if it still works.  It does.
  3/18/15 Made AdrToHexStr also a function.
*/

struct FullName IS
  FullName PtrType PrevP;
  FullName PtrType NextP;
  char FirstName[20];
  char Initial;
  char LastName[20];
END; // struct FullName

typedef FullName PtrType FullNamePointerType;

FullNamePointerType StartOfListP,EndofListP,CurrentPlaceInListP,PrevPlaceInListP,AnEntryPointer;
int I;
string s,s0;

/******************************************************************************************************/

RETURN string FUNCTION AdrToHexStr(ADDRESS adr, string REF OutStr) IS

const int ASCZERO = '0';
const int ascA    = 'A';
int i,j,h;
unsigned long L;
string Str20="";

  i = 0;
  L = (unsigned long) adr;
  REPEAT /* until L = 0 */
    h = L MOD 16;
    IF (h <= 9) THEN
      Str20 += (h + ASCZERO);
    ELSE
      Str20 += (h -10 + ascA);
    ENDIF; // n <= 9
    ++i;
    L = L / 16;
  UNTIL (L EQ 0);
  j = 1;  /* first posn is a space to leave room for sign char */
  OutStr = " ";
  REPEAT /* until i = 0 */
    --i;
    OutStr += Str20.at(i);
    ++j;
  UNTIL (i EQ 0);
  OutStr += "\0";
  Str20 = OutStr;
  return Str20;
ENDPROC; // AdrToHexStr

/**************************************************************************************************/

int main(argcargv) IS

  StartOfListP = nullptr;
  EndofListP = NULL;
  CurrentPlaceInListP = NULL;
  PrevPlaceInListP = NULL;

                  /* Generate the first name in the list */
  AnEntryPointer = new FullName;
  StartOfListP = AnEntryPointer;
  cout << " 1: ";
  s0 = AdrToHexStr(AnEntryPointer,s);
  cout << s.c_str() << " (" << s0.c_str() << ") ";

  AnEntryPointer->PrevP = NULL;
  strcpy(AnEntryPointer->FirstName, "John ");
  AnEntryPointer->Initial = 'Q';
  strcpy(AnEntryPointer->LastName, " Doe");
  AnEntryPointer->NextP = nullptr;

               /* Generate 2nd name in the list */
  PrevPlaceInListP = AnEntryPointer;
  AnEntryPointer = new FullName;
  cout << ", 2: ";
  AdrToHexStr(AnEntryPointer,s);
  cout << s.c_str();
  CurrentPlaceInListP = AnEntryPointer;
  PrevPlaceInListP->NextP = CurrentPlaceInListP;
  CurrentPlaceInListP->PrevP = PrevPlaceInListP;
  strcpy(CurrentPlaceInListP->FirstName, "Mary ");
  CurrentPlaceInListP->Initial = 'R';
  strcpy(CurrentPlaceInListP->LastName, " Johnson");
  CurrentPlaceInListP->NextP = NULL;

                 /* Add 10 more names to complete the list */
  FOR I=1; I<=10; ++I DO
    PrevPlaceInListP = CurrentPlaceInListP;
    AnEntryPointer = new FullName;
    cout << ", ";
    cout << I+2;
    cout << ":";
    AdrToHexStr(AnEntryPointer,s);
    cout << s.c_str();
    CurrentPlaceInListP = AnEntryPointer;
    PrevPlaceInListP->NextP = CurrentPlaceInListP;
    CurrentPlaceInListP->PrevP = PrevPlaceInListP;
    strcpy(CurrentPlaceInListP->FirstName, "Billy ");
//                                                     CurrentPlaceInListP->Initial = (char) I+64;   // 65 is cap A
    CurrentPlaceInListP->Initial = I+64;   // 65 is cap A
    strcpy(CurrentPlaceInListP->LastName, " Franklin");
    CurrentPlaceInListP->NextP = NULL;
  ENDFOR; /* FOR I */
  EndofListP = CurrentPlaceInListP;
  cout << endl << endl;

                        /* Display the list on the monitor in forward direction */
  cout << " List in forward direction." << endl;
  CurrentPlaceInListP = StartOfListP;
  WHILE CurrentPlaceInListP NE nullptr DO
    AdrToHexStr(CurrentPlaceInListP->PrevP,s);
    cout << s.c_str();
    cout << ": ";
    cout << CurrentPlaceInListP->FirstName << CurrentPlaceInListP->Initial << CurrentPlaceInListP->LastName << " : ";
    AdrToHexStr(CurrentPlaceInListP->NextP,s);
    cout << s.c_str() << endl;
    PrevPlaceInListP = CurrentPlaceInListP;
    CurrentPlaceInListP = CurrentPlaceInListP->NextP;
  ENDWHILE;

                        /* Display the list on the monitor in reverse direction */
  cout << endl << " List in reverse direction. " << endl;
  CurrentPlaceInListP = EndofListP;
  WHILE CurrentPlaceInListP NE NULL DO
     AdrToHexStr(CurrentPlaceInListP->PrevP,s);
     cout << s << ": ";
     cout << CurrentPlaceInListP->FirstName << CurrentPlaceInListP->Initial << CurrentPlaceInListP->LastName << " : ";
     AdrToHexStr(CurrentPlaceInListP->NextP,s);
     cout << s.c_str() << endl;
     PrevPlaceInListP = CurrentPlaceInListP;
     CurrentPlaceInListP = CurrentPlaceInListP->PrevP;
  ENDWHILE;


                             /* Deallocate is a good habit */
  AnEntryPointer = StartOfListP;
  WHILE AnEntryPointer NE NULL DO
     CurrentPlaceInListP = AnEntryPointer->NextP;
     delete AnEntryPointer;
     AnEntryPointer = CurrentPlaceInListP;
  ENDWHILE;

ENDFUNC; // LinkList
