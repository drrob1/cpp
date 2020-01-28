#define NCURSESON
#include "/home/rob/mymacros.h"

#include <panel.h>

#define NLINES 10
#define NCOLS 40

PROCEDURE init_wins(WINDOW **wins, int n);
PROCEDURE win_show(WinPtrType win, CharPointerType label, int label_color);
PROCEDURE print_in_middle(WinPtrType win, int starty, int startx, int width, CharPointerType string, chtype color);

int main() IS

	ARRAYOF WinPtrType my_wins[3];
	ARRAYOF PanelPtrType my_panels[3];
	        PanelPtrType top;
	int ch;

	/* Initialize curses */
	initscr();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	/* Initialize all the colors */
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);

	init_wins(my_wins, 3);  // this procedure is defined next.
	
	/* Attach a panel to each window */ 	/* Order is bottom up */
	my_panels[0] = new_panel(my_wins[0]); 	/* Push 0, order: stdscr-0 */
	my_panels[1] = new_panel(my_wins[1]); 	/* Push 1, order: stdscr-0-1 */
	my_panels[2] = new_panel(my_wins[2]); 	/* Push 2, order: stdscr-0-1-2 */

	/* Set up the user pointers to the next panel */
	set_panel_userptr(my_panels[0], my_panels[1]);
	set_panel_userptr(my_panels[1], my_panels[2]);
	set_panel_userptr(my_panels[2], my_panels[0]);

	/* Update the stacking order.  2nd panel will be on top */
	update_panels();

	/* Show it on the screen */
	attron(COLOR_PAIR(4));
	mvprintw(LINES - 2, 0, "Use tab to browse through the windows (F1 to Exit)");
	attroff(COLOR_PAIR(4));
	doupdate();

	top = my_panels[2];
        ch = getch();
	WHILE ch NE KEY_F(1) DO
		switch(ch) OF
			case 9:
				top = (PanelPtrType) panel_userptr(top);     // this is a type cast.  And I don't know about panel_userptr.
				top_panel(top);
				break;
		ENDCASE;
		update_panels();
		doupdate();
                ch = getch();
	ENDWHILE;
	endwin();
	return 0;
END; // end Main

/* Put all the windows */
/* Called like this in main above:   init_wins(my_wins, 3); */
PROCEDURE init_wins(WINDOW **wins, int n)
{
	int x, y, i;
	ARRAYOF char label[80];

	y = 2;
	x = 10;
	for(i = 0; i < n; ++i)
	{
		wins[i] = newwin(NLINES, NCOLS, y, x);
		sprintf(label, "Window Number %d", i + 1);
		win_show(wins[i], label, i + 1);
		y += 3;
		x += 7;
	}
ENDPROC;

/* Show the window with a border and a label */
//                            PROCEDURE win_show(WINDOW *win, char *label, int label_color)
PROCEDURE win_show(WinPtrType win, CharPointerType label, int label_color)
{
	int startx, starty, height, width;

	getbegyx(win, starty, startx);
	getmaxyx(win, height, width);

	box(win, 0, 0);
	mvwaddch(win, 2, 0, ACS_LTEE); 
	mvwhline(win, 2, 1, ACS_HLINE, width - 2); 
	mvwaddch(win, 2, width - 1, ACS_RTEE); 
	
	print_in_middle(win, 1, 0, width, label, COLOR_PAIR(label_color));
ENDPROC;

// PROCEDURE print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
PROCEDURE print_in_middle(WinPtrType win, int starty, int startx, int width, CharPointerType string, chtype color)
{
	int length, x, y;
	float temp;

	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	wattron(win, color);
	mvwprintw(win, y, x, "%s", string);
	wattroff(win, color);
	refresh();
ENDPROC;


