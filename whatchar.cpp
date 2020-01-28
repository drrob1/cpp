#define NCURSESON
#include "macros.h"

int main(int argc, char *argv[]) IS
  ARRAYOF char line[80];
  string s;
  int ch,x,y,old;

  initscr();
  cbreak();
  noecho();
  nonl();
  intrflush(stdscr,FALSE);
  keypad(stdscr,TRUE);
  IF NOT has_colors() THEN
    endwin();
//    line = "Your terminal does not support color\n";  error saying incompatible types assigning const char[38] to char[80]
    strcpy(line,"Your terminal does not support color\n");
    printf("%s",line);
//    printf("Your terminal does not support color\n");
    exit(-1);
  ELSIF NOT can_change_color() THEN
    printf("Your terminal cannot change colors\n");
  ENDIF;
  s = "Will now show lines in different colors.  ";
  printw("%s",s.c_str());
  printw("COLORS value is %d, COLOR_PAIRS value is %d. \n",COLORS,COLOR_PAIRS);
  strcpy(line," Testing c-strings for output\n\n");
  printw("%s",line);

  start_color();
// After this init call, there are 8 colors which are already defined, and 64 color pairs that can be defined with init_pair() calls.
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_BLACK, COLOR_RED);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_BLACK, COLOR_YELLOW);
  init_pair(5, COLOR_CYAN, COLOR_BLACK);
  init_pair(6, COLOR_BLACK, COLOR_CYAN);
  init_pair(7, COLOR_GREEN, COLOR_BLACK);
  init_pair(8, COLOR_BLACK, COLOR_GREEN);
  init_pair(9, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(10, COLOR_BLACK, COLOR_MAGENTA);
  init_pair(11, COLOR_BLUE, COLOR_BLACK);
  init_pair(12, COLOR_BLACK, COLOR_BLUE);
  init_pair(13, COLOR_WHITE, COLOR_BLACK);
  int a = 14;
  init_pair(a, COLOR_BLACK, COLOR_WHITE);

  int c = 0;
  attron(COLOR_PAIR(c));
//  attron(COLOR_PAIR(0));
  printw("%2d: COLORS value is %d, COLOR_PAIRS value is %d. \n",c,COLORS,COLOR_PAIRS);
//  attroff(COLOR_PAIR(c));   Testing to see if I really need to turn this color attribute off before turning on a different one.
  ++c;
  attron(COLOR_PAIR(c));
  printw("%2d: COLORS value is %d, COLOR_PAIRS value is %d. ",c,COLORS,COLOR_PAIRS);
//  attroff(COLOR_PAIR(c));  Looks like I don't have to turn this color attribute off before turning on the next one.
  ++c;
  attron(COLOR_PAIR(c));
  printw("%2d: COLORS value is %d, COLOR_PAIRS value is %d. \n",c,COLORS,COLOR_PAIRS);
  attroff(COLOR_PAIR(c));
  ++c;
  attron(COLOR_PAIR(c));
  printw("%2d: COLORS value is %d, COLOR_PAIRS value is %d. ",c,COLORS,COLOR_PAIRS);
  attroff(COLOR_PAIR(c));
  ++c;
  attron(COLOR_PAIR(c));
  printw("%2d: COLORS value is %d, COLOR_PAIRS value is %d. \n",c,COLORS,COLOR_PAIRS);
  attroff(COLOR_PAIR(c));
  ++c;
  attron(COLOR_PAIR(c));
  printw("%2d: COLORS value is %d, COLOR_PAIRS value is %d. ",c,COLORS,COLOR_PAIRS);
  attroff(COLOR_PAIR(c));
  ++c;
  attron(COLOR_PAIR(c));
  printw("%2d: COLORS value is %d, COLOR_PAIRS value is %d. \n",c,COLORS,COLOR_PAIRS);
  attroff(COLOR_PAIR(c));
  ++c;
  attron(COLOR_PAIR(c));
  printw("%2d: COLORS value is %d, COLOR_PAIRS value is %d. ",c,COLORS,COLOR_PAIRS);
  attroff(COLOR_PAIR(c));
  ++c;
  attron(COLOR_PAIR(c));
  printw("%2d: COLORS value is %d, COLOR_PAIRS value is %d. \n",c,COLORS,COLOR_PAIRS);
  attroff(COLOR_PAIR(c));
  ++c;
  attron(COLOR_PAIR(c));
  printw("%2d: COLORS value is %d, COLOR_PAIRS value is %d. ",c,COLORS,COLOR_PAIRS);
  attroff(COLOR_PAIR(c));
  ++c;
  attron(COLOR_PAIR(c));
  printw("%2d: COLORS value is %d, COLOR_PAIRS value is %d. \n",c,COLORS,COLOR_PAIRS);
  attroff(COLOR_PAIR(c));
  ++c;
  attron(COLOR_PAIR(c));
  printw("%2d: COLORS value is %d, COLOR_PAIRS value is %d. ",c,COLORS,COLOR_PAIRS);
  attroff(COLOR_PAIR(c));
  ++c;
  attron(COLOR_PAIR(c));
  printw("%2d: COLORS value is %d, COLOR_PAIRS value is %d. \n",c,COLORS,COLOR_PAIRS);
  attroff(COLOR_PAIR(c));
  ++c;
  attron(COLOR_PAIR(c));
  printw("%2d: COLORS value is %d, COLOR_PAIRS value is %d. ",c,COLORS,COLOR_PAIRS);
  attroff(COLOR_PAIR(c));
  ++c;
  attron(COLOR_PAIR(c));
  printw("%2d: COLORS value is %d, COLOR_PAIRS value is %d. \n",c,COLORS,COLOR_PAIRS);
  attroff(COLOR_PAIR(c));

  getmaxyx(stdscr,y,x);
  printw(" LINES= %d, COLS= %d, rows= %d, columns= %d.\n",LINES,COLS,y,x);
  refresh();

  old=0;
  WHILE true DO
    printw(" Press any key : ");
    ch = getch();
    printw(" Key pressed decimal value : %d, octal value : %o, hex value %x. \n",ch,ch,ch);
    IF ch EQ old THEN
      break;  // exit if same char hit twice in a row.
    ELSE
      old = ch;
    ENDIF;
  ENDWHILE;
  printw(" exiting ... ");
  getch();
  nl();
  nocbreak();
  echo();
  endwin();
  return 0;
ENDFUNC;
