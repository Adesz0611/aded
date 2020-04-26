#include <stdlib.h>
#include <string.h>

#include "curses.h"

void curses_init(void)
{
    // Initialize NCURSES
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    
    curses = (curses_t *)malloc(sizeof(*curses));
    memset(curses, 0, sizeof(*curses));

    getmaxyx(stdscr, curses->termY, curses->termX);
}

void curses_one_loop(void)
{
    getch();
}

void curses_clean(void)
{
    endwin();
}
