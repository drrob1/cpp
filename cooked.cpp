#define NCURSESON
#include "macros.h"

int main() IS
        int c;
        initscr();                      /* Start curses mode              */
        clear();
        noecho();
        cbreak();       //Line buffering disabled. pass on everything
        nonl();
        intrflush(stdscr,FALSE);
        keypad(stdscr,TRUE);
        start_color();                  /* Start color                  */
        init_pair(1, COLOR_RED, COLOR_BLACK);

        attron(COLOR_PAIR(1));
        printw("Hello World !!!  Will enter cooked mode after <enter> \n "); // Print Hello World
        
        refresh();                      /* Print it on to the real screen */
        c = getch();                    // pause
        def_prog_mode();                /* Save the tty modes             */
        endwin();                       /* End curses mode temporarily    */
        system("sh");                   /* Do whatever you like in cooked mode */
//        system("/bin/sh");  this syntax works as well as the shorter one does
        reset_prog_mode();              /* Return to the previous tty mode*/
                                        /* stored by def_prog_mode()      */
        refresh();                      /* Do refresh() to restore the    */
                                        /* Screen contents                */
        init_pair(2, COLOR_RED, COLOR_WHITE);
        attron(COLOR_PAIR(2));
        printw("Back from shelling out\n");     /* Back to curses use the full    */
        refresh();                      /* capabilities of curses         */
        c = getch();                    // pause
        endwin();                       /* End curses mode                */

        return 0;
END;
