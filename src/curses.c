#include <stdlib.h>
#include <string.h>

#include "curses.h"
#include "line.h"
#include "buffer.h"
#include "debug.h"
#include "defs.h"
#include "statusbar.h"
#include "cursor.h"
#include "display.h"

TerminalInfo *termInfo;
ADED_WINDOW *main_window;

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

void curses_windowDelete(ADED_WINDOW *win)
{
    delwin(win->window); // NCURSES' function
    free(win);
}

void curses_windowResize(ADED_WINDOW *win, int height, int width)
{
    wresize(win->window, height, width);
    win->height = height;
    win->width  = width;
}

void curses_resize()
{
    // Ncurses' docs says that you should call endwin(), 
    // followed by a refresh() to properly update curses
    endwin();
    refresh();

    // Get the terminal's new size
    getmaxyx(stdscr, termInfo->height, termInfo->width);
   
    // Resize the main window
    curses_windowResize(main_window, termInfo->height - 1, termInfo->width);
    
    // Resize the statusbar
    wresize(statusbar->window->window, 1, termInfo->width);
    statusbar->window->width = termInfo->width;
    mvwin(statusbar->window->window, termInfo->height - 1, 0);

    DEBUGF("termInfo->height (y) = %d\n", termInfo->height);
    DEBUGF("termInfo->width (x) = %d\n\n", termInfo->width);

    // Check vertically
    if(offset->line_yOffset->next != NULL && cursor->cursY + 1 > main_window->height)
    {
        offset->line_yOffset = offset->line_yOffset->next;
        cursor->cursY--;
    }


    // Check horizontally
    if(line_current->buffer[buffer->cursX - XSCROLL_VALUE] != '\0' && cursor->cursX + 1 > main_window->width)
    {
        offset->xOffset += XSCROLL_VALUE;
        cursor->cursX -= XSCROLL_VALUE;
    }
    else if(line_current->buffer[buffer->cursX - 1] != '\0' && cursor->cursX + 1 > main_window->width)
    {
        offset->xOffset++;
        cursor->cursX--;
    }

    display_buffer(main_window, offset->line_yOffset, 0, main_window->height);
}

void curses_clean(void)
{
    free(termInfo);
    curses_windowDelete(main_window);
    endwin();
}
