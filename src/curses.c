#include <stdlib.h>
#include <string.h>

#include "curses.h"
#include "line.h"
#include "buffer.h"
#include "debug.h"
#include "defs.h"
#include "statusbar.h"
#include "cursor.h"

void curses_init(void)
{
    // Initialize NCURSES
    initscr();
    cbreak();
    noecho();
    
    // Allocate memory for the terminal info struct
    termInfo = (TerminalInfo *)malloc(sizeof(*termInfo));
    memset(termInfo, 0, sizeof(*termInfo));

    // Get the terminal size
    getmaxyx(stdscr, termInfo->height, termInfo->width);
    
    // Create the main window
    main_window = curses_windowAdd(termInfo->height - 1, termInfo->width, 0, 0);

    // Start debugging
    DEBUGF("Started\n");
}

ADED_WINDOW *curses_windowAdd(int height, int width, int startY, int startX)
{
    ADED_WINDOW *tmp;

    tmp = (ADED_WINDOW *)malloc(sizeof(*tmp));
    memset(tmp, 0, sizeof(*tmp));

    tmp->window = newwin(height, width, startY, startX);
    tmp->height = height;
    tmp->width = width;

    keypad(tmp->window, true);

    return tmp;
}

void curses_resize()
{
    endwin();
    refresh();
    getmaxyx(stdscr, termInfo->height, termInfo->width);
    
    wresize(main_window->window, termInfo->height - 1, termInfo->width);
    main_window->height = termInfo->height - 1;
    main_window->width = termInfo->width;

    wresize(statusbar->window->window, 1, termInfo->width);
    statusbar->window->width = termInfo->width;
    mvwin(statusbar->window->window, termInfo->height - 1, 0);

    DEBUGF("termInfo->height (y) = %d\n", termInfo->height);
    DEBUGF("termInfo->width (x) = %d\n\n", termInfo->width);

    // FIXME: 
    if(offset->line_yOffset->next != NULL && cursor->cursY + 1 > main_window->height)
    {
        offset->line_yOffset = offset->line_yOffset->next;
        cursor->cursY--;
    }
}

void curses_clean(void)
{
    endwin();
}
