#define NCURSESON
#include "macros.h"
// #include <ncurses.h>

#define WIDTH 20
#define HEIGHT 9

int startx = 0;
int starty = 0;
int r=20;

const char *choices[] = {"Choice 1", "Choice 2", "Choice 3", "Choice 4", "Exit", };
int n_choices = sizeof(choices) / sizeof(char *);

PROCEDURE print_menu(WINPTRTYPE menu_win, int highlight);
PROCEDURE report_choice(int mouse_x, int mouse_y, int REF choice);

int main() IS
        int c, choice = 0;
        WINPTRTYPE menu_win;
        MEVENT event;

        /* Initialize curses */
        initscr();
        clear();
        noecho();
        cbreak();       //Line buffering disabled. pass on everything
        nonl();
        intrflush(stdscr,FALSE);
        keypad(stdscr,TRUE);

        /* Try to put the window in the middle of screen */
        startx = (80 - WIDTH) / 2;
        starty = (24 - HEIGHT) / 2;
        
        attron(A_REVERSE);
        mvprintw(15, 1, "Click on Exit to quit (Works best in a virtual console)");
        refresh();
        attroff(A_REVERSE);

        /* Print the menu for the first time */
//        menu_win = newwin(HEIGHT, WIDTH, starty, startx);
//        IF menu_win EQ nullptr THEN
//          mvprintw(1,1," menu_win is null.  Will exit now.");
//          return -1;
//        ENDIF;
//        print_menu(stdscr, 1);
        /* Get all the mouse events */
        mousemask(ALL_MOUSE_EVENTS BITOR REPORT_MOUSE_POSITION, NULL);
        
        WHILE TRUE DO 
          c = getch();
          mvprintw(0,1," ch entered is %d   ",c);
          refresh();
          switch(c)
          {
            case KEY_MOUSE:
              IF getmouse(ADROF event) EQ OK THEN   
                IF event.bstate BITAND BUTTON1_CLICKED THEN  /* When the user clicks left mouse button */
                  mvprintw(2,1," Button1_clicked  event.x is %d , event.y is %d",event.x,event.y);
//                  report_choice(event.x + 1, event.y + 1, choice);
//                  IF choice EQ -1 THEN //Exit chosen
//                    goto end;
//                  ENDIF; // if choice is Exit
//                  mvprintw(22, 1, "Choice made is : %d String Chosen is \"%10s\"", choice, choices[choice - 1]);
                  refresh(); 
                ENDIF; // if event.bstate button1 pressed
              ELSE
                mvprintw(30,1, " getmouse is not OK, for whatever reason.");
              ENDIF; // if getmouse EQ OK
//              print_menu(menu_win, choice);
              break;
            case 'Q': goto end;
            case 'E': goto end;
            case KEY_END: goto end;
            default :
              mvprintw(r,1, " in default case of switch(c).  event id %d, x %d y %d z %d  bstate %x  ",event.id, event.x,event.y,event.z,event.bstate);
            ++r;
          ENDCASE;
          refresh();
        ENDWHILE;               
end:
        endwin();
        return 0;
ENDPROC;


// void print_menu(WINDOW *menu_win, int highlight)
PROCEDURE print_menu(WINPTRTYPE menu_win, int highlight) IS
        int x, y, i;    

        x = 2;
        y = 2;
        box(menu_win, 0, 0);
        for(i = 0; i < n_choices; ++i)
        {
                if(highlight EQ i + 1)
                {
                        wattron(menu_win, A_REVERSE); 
                        mvwprintw(menu_win, y, x, "%s", choices[i]);
                        wattroff(menu_win, A_REVERSE);
                }
                else
                        mvwprintw(menu_win, y, x, "%s", choices[i]);
                ++y;
        }
        wrefresh(menu_win);
ENDPROC;

/* Report the choice according to mouse position */
// void report_choice(int mouse_x, int mouse_y, int *p_choice)
PROCEDURE report_choice(int mouse_x, int mouse_y, int REF choice) IS
        int i,j;

        i = startx + 2;
        j = starty + 3;
        
        for(choice = 0; choice < n_choices; ++choice)
                if(mouse_y EQ j + choice AND mouse_x >= i AND mouse_x <= i + strlen(choices[choice]))
                {
                        if(choice EQ n_choices - 1)
                                choice = -1;         
                        else
                                choice = choice + 1; 
                        break;
                }
ENDPROC;
