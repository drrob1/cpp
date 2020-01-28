#define NCURSESON
#include "macros.h"

PROCEDURE print_in_middle(WINPTRTYPE win, int starty, int startx, int width, const char *str);

int main(int argc, char *argv[]) IS
        initscr();                      /* Start curses mode            */
        IF has_colors() EQ FALSE THEN
                endwin();
                printf("Your terminal does not support color\n");
                exit(1);
        ENDIF;
        start_color();                  /* Start color                  */
        init_pair(1, COLOR_RED, COLOR_BLACK);

        attron(COLOR_PAIR(1));
        printw("WTF");
        print_in_middle(stdscr, LINES / 2, 0, 0, "Viola !!! In color ...");
        attroff(COLOR_PAIR(1));
        getch();
        endwin();
ENDFUNC;

PROCEDURE print_in_middle(WINPTRTYPE win, int starty, int startx, int width, const char *str) IS
        int length, x, y;
        float temp;

        IF win EQ NULL THEN
                win = stdscr;
        ENDIF;
        getyx(win, y, x);
        IF startx >= 0 THEN
                x = startx;
        ENDIF;
        IF starty >= 0 THEN
                y = starty;
        ENDIF;
        IF width EQ 0 THEN
                width = 80;
        ENDIF;

        length = strlen(str);
        temp = (width - length)/ 2;
        x = startx + (int)temp;
        mvwprintw(win, y, x, "%s", str);
        refresh();
ENDPROC;
