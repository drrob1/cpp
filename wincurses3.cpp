#define NCURSESON
#include "macros.h"
//                           #include <ncurses.h>

typedef struct _win_border_struct {
        chtype  ls, rs, ts, bs, 
                tl, tr, bl, br;
}WIN_BORDER;

typedef struct _WIN_struct {

        int startx, starty;
        int height, width;
        WIN_BORDER border;
}WIN;

PROCEDURE init_win_params(WIN POINTINGBACKTO p_win);
PROCEDURE print_win_params(WIN POINTINGBACKTO   p_win);
PROCEDURE create_box(WIN POINTINGBACKTO win, bool flag);

int main(int argc, char *argv[]) IS
        WIN win;
        int ch;

        initscr();                      /* Start curses mode            */
        start_color();                  /* Start the color functionality */
        cbreak();                       /* Line buffering disabled, Pass on
                                         * everty thing to me           */
        keypad(stdscr, TRUE);           /* I need that nifty F1         */
        noecho();
        init_pair(1, COLOR_CYAN, COLOR_BLACK);

        /* Initialize the window parameters */
        init_win_params(ADROF win);
        print_win_params(ADROF win);

        attron(COLOR_PAIR(1));
        printw("Press F1 to exit");
        refresh();
        attroff(COLOR_PAIR(1));
        
        create_box(ADROF win, TRUE);
        ch = getch();
        WHILE ch NE KEY_F(1) DO
                switch(ch)
                {       case KEY_LEFT:
                                create_box(ADROF win, FALSE);
                                --win.startx;
                                create_box(ADROF win, TRUE);
                                break;
                        case KEY_RIGHT:
                                create_box(ADROF win, FALSE);
                                ++win.startx;
                                create_box(ADROF win, TRUE);
                                break;
                        case KEY_UP:
                                create_box(ADROF win, FALSE);
                                --win.starty;
                                create_box(ADROF win, TRUE);
                                break;
                        case KEY_DOWN:
                                create_box(ADROF win, FALSE);
                                ++win.starty;
                                create_box(ADROF win, TRUE);
                                break;  
                }
        ch = getch();
        ENDWHILE;
        endwin();                       /* End curses mode                */
        return 0;
ENDFUNC;


PROCEDURE init_win_params(WIN POINTINGBACKTO p_win) IS
        p_win->height = 3;
        p_win->width = 10;
        p_win->starty = (LINES - p_win->height)/2;      
        p_win->startx = (COLS - p_win->width)/2;

        p_win->border.ls = '|';
        p_win->border.rs = '|';
        p_win->border.ts = '-';
        p_win->border.bs = '-';
        p_win->border.tl = '+';
        p_win->border.tr = '+';
        p_win->border.bl = '+';
        p_win->border.br = '+';
ENDPROC;

PROCEDURE print_win_params(WIN POINTINGBACKTO p_win) IS
#ifdef _DEBUG
        mvprintw(25, 0, "%d %d %d %d", p_win->startx, p_win->starty, 
                                p_win->width, p_win->height);
        refresh();
#endif
ENDPROC;

PROCEDURE create_box(WIN POINTINGBACKTO p_win, bool flag) IS
        int i, j;
        int x, y, w, h;

        x = p_win->startx;
        y = p_win->starty;
        w = p_win->width;
        h = p_win->height;

        if(flag EQ TRUE)
        {       mvaddch(y, x, p_win->border.tl);
                mvaddch(y, x + w, p_win->border.tr);
                mvaddch(y + h, x, p_win->border.bl);
                mvaddch(y + h, x + w, p_win->border.br);
                mvhline(y, x + 1, p_win->border.ts, w - 1);
                mvhline(y + h, x + 1, p_win->border.bs, w - 1);
                mvvline(y + 1, x, p_win->border.ls, h - 1);
                mvvline(y + 1, x + w, p_win->border.rs, h - 1);

        }
        else
                for(j = y; j <= y + h; ++j)
                        for(i = x; i <= x + w; ++i)
                                mvaddch(j, i, ' ');
                                
        refresh();

ENDPROC;

