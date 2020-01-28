#define NCURSESON
#include "macros.h"

int main(int argc, char *argv[]) IS
  initscr();
  cbreak();
  noecho();
  nonl();
  intrflush(stdscr,FALSE);
  keypad(stdscr,TRUE);
  IF NOT has_colors() THEN
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  ELSIF NOT can_change_color() THEN
    printf("Your terminal cannot change colors\n");
  ENDIF;
  printw("COLORS value is %d, COLOR_PAIRS value is %d. \n",COLORS,COLOR_PAIRS);

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
  init_pair(14, COLOR_BLACK, COLOR_WHITE);

  attron(COLOR_PAIR(0));
  printw("0: COLORS value is %d, COLOR_PAIRS value is %d. \n ",COLORS,COLOR_PAIRS);
  attroff(COLOR_PAIR(0));
  attron(COLOR_PAIR(1));
  printw("1: COLORS value is %d, COLOR_PAIRS value is %d. ",COLORS,COLOR_PAIRS);
  attroff(COLOR_PAIR(1));
  attron(COLOR_PAIR(2));
  printw("2: COLORS value is %d, COLOR_PAIRS value is %d. \n",COLORS,COLOR_PAIRS);
  attroff(COLOR_PAIR(2));
  attron(COLOR_PAIR(3));
  printw("3: COLORS value is %d, COLOR_PAIRS value is %d. ",COLORS,COLOR_PAIRS);
  attroff(COLOR_PAIR(3));
  attron(COLOR_PAIR(4));
  printw("4: COLORS value is %d, COLOR_PAIRS value is %d. \n",COLORS,COLOR_PAIRS);
  attroff(COLOR_PAIR(4));
  attron(COLOR_PAIR(5));
  printw("5: COLORS value is %d, COLOR_PAIRS value is %d. ",COLORS,COLOR_PAIRS);
  attroff(COLOR_PAIR(5));
  attron(COLOR_PAIR(6));
  printw("6: COLORS value is %d, COLOR_PAIRS value is %d. \n",COLORS,COLOR_PAIRS);
  attroff(COLOR_PAIR(6));
  attron(COLOR_PAIR(7));
  printw("7: COLORS value is %d, COLOR_PAIRS value is %d. ",COLORS,COLOR_PAIRS);
  attroff(COLOR_PAIR(7));
  attron(COLOR_PAIR(8));
  printw("8: COLORS value is %d, COLOR_PAIRS value is %d. \n",COLORS,COLOR_PAIRS);
  attroff(COLOR_PAIR(8));
  attron(COLOR_PAIR(9));
  printw("9: COLORS value is %d, COLOR_PAIRS value is %d. ",COLORS,COLOR_PAIRS);
  attroff(COLOR_PAIR(9));
  attron(COLOR_PAIR(10));
  printw("10: COLORS value is %d, COLOR_PAIRS value is %d. \n",COLORS,COLOR_PAIRS);
  attroff(COLOR_PAIR(10));
  attron(COLOR_PAIR(11));
  printw("11: COLORS value is %d, COLOR_PAIRS value is %d. ",COLORS,COLOR_PAIRS);
  attroff(COLOR_PAIR(11));
  attron(COLOR_PAIR(12));
  printw("12: COLORS value is %d, COLOR_PAIRS value is %d. \n",COLORS,COLOR_PAIRS);
  attroff(COLOR_PAIR(12));
  attron(COLOR_PAIR(13));
  printw("13: COLORS value is %d, COLOR_PAIRS value is %d. ",COLORS,COLOR_PAIRS);
  attroff(COLOR_PAIR(13));
  attron(COLOR_PAIR(14));
  printw("14: COLORS value is %d, COLOR_PAIRS value is %d. \n",COLORS,COLOR_PAIRS);
  attroff(COLOR_PAIR(14));
  getch();
  endwin();
ENDFUNC;
