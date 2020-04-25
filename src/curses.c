#include <stdlib.h>
#include <string.h>

#include "curses.h"
#include "init.h"


void curses_init(void)
{
    // Initialize NCURSES
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    
    init_t = (Init *)malloc(sizeof(*init_t));
    memset(init_t, 0, sizeof(*init_t));

    getmaxyx(stdscr, init_t->termY, init_t->termX);
}

void curses_one_loop(void)
{
    getch();
}

void curses_clean(void)
{
    endwin();
}
