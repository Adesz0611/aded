#include <stdlib.h>
#include <string.h>

#include "curses.h"
#include "line.h"
#include "buffer.h"
#include "debug.h"

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

    debugf("Started\n");
}

void curses_resize()
{
    getmaxyx(stdscr, curses->termY, curses->termX);
    wresize(curses->window, curses->termY, curses->termX);

    debugf("curses->termY = %d\n", curses->termY);
    debugf("curses->termX = %d\n\n", curses->termX);

    // FIXME: 
    if(line_yOffset->next != NULL && buffer->cursY + 1 > curses->termY)
    {
        line_yOffset = line_yOffset->next;
        buffer->cursY--;
    }
}

void curses_clean(void)
{
    endwin();
}
