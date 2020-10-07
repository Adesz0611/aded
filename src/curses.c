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

WINDOW *main_window;

void curses_init(void)
{
    // Initialize NCURSES
    initscr();
    cbreak();
    noecho();
   
    // Initialize main window
    main_window = newwin(STDSCR_HEIGHT - 1, STDSCR_WIDTH, 0, 0);
    keypad(main_window, true);

    // Start debugging
    DEBUGF("Started\n");
}

void curses_resize()
{
    // Ncurses' docs says that you should call endwin(), 
    // followed by a refresh() to properly update curses
    endwin();
    refresh();
    clear();

    // Resize the main window
    wresize(main_window, STDSCR_HEIGHT - 1, STDSCR_WIDTH);
    wnoutrefresh(main_window);

    // Resize the statusbar
    mvwin(statusbar->window, STDSCR_HEIGHT - 1, 0);
    wnoutrefresh(statusbar->window);

    DEBUGF("Standard screen's height (y) = %d\n", STDSCR_HEIGHT);
    DEBUGF("Standard screen's width (x) = %d\n\n", STDSCR_WIDTH);

    // Check vertically
    if(cursor->cursY + 1 > WINDOW_HEIGHT(main_window))
    {
        int tmp_diff = cursor->cursY + 1 - WINDOW_HEIGHT(main_window);
        for(int i = 0; i < tmp_diff; i++)
        {
            buffer->line_yOffset = buffer->line_yOffset->next;
        }
        cursor->cursY = WINDOW_HEIGHT(main_window) - 1;
    }


    // Check horizontally
    if(cursor->cursX + 1 > WINDOW_WIDTH(main_window))
    {
        buffer->xOffset = buffer->cursX - WINDOW_WIDTH(main_window) + XSCROLL_VALUE;
        cursor->cursX = WINDOW_WIDTH(main_window) - XSCROLL_VALUE;
    }

    display_buffer(main_window, buffer->line_yOffset, 0, WINDOW_HEIGHT(main_window));
}

void curses_clean(void)
{
    delwin(main_window);
    endwin();
}
