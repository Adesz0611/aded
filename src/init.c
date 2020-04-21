/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#include <ncurses.h>

#include "init.h"

void init(void)
{
    initscr();
    cbreak();
    noecho();
}

void clean(void)
{
    endwin();
}
