#include <stdlib.h>
#include <string.h>

#include "curses.h"

void curses_init(void)
{
    // Initialize NCURSES
    initscr();
    cbreak();
    noecho();
    
    curses = (curses_t *)malloc(sizeof(*curses));
    memset(curses, 0, sizeof(*curses));

    getmaxyx(stdscr, curses->termY, curses->termX);
   
    curses->window = newwin(curses->termY, curses->termX, 0, 0);
    keypad(curses->window, true);
}

#if __unix__
#ifdef SIGWINCH
void curses_resize()
{
    getmaxyx(stdscr, curses->termY, curses->termX);
    wresize(curses->window, curses->termY, curses->termX); 
}
#endif
#endif

void curses_clean(void)
{
    endwin();
}
