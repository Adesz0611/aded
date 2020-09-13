/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#include <stdio.h>

#include "display.h"
#include "line.h"
#include "curses.h"
#include "file.h"
#include "defs.h"

void display_buffer(WINDOW *win, line_t *line, int y, int nrow)
{
    int i;
    line_t *tmp = line;
    
    for(i = y; i < y + nrow; i++)
    {
        display_blankRow(win, i, 0, WINDOW_WIDTH(win));
    }

    for(i = y; i < y + nrow && tmp != NULL; i++, tmp = tmp->next)
    {
        display_line(win, tmp, i, 0, WINDOW_WIDTH(win));
    }
}

/*-- Blank--*/

/*  This function puts n spaces to the terminal */
void display_blankRow(WINDOW *win, int y, int x, int n)
{
    wmove(win, y, x);

    for(; n > 0; n--)
        waddch(win, ' ');
}

/*-- Draw --*/

/* Display n characters of a line*/
void display_line(WINDOW *win, line_t *line, int y, int x, int n)
{
    /*
    for(int i = x; i < x + n && line->buffer[i] != '\0'; i++)
    {
            #if DISPLAY_PILCROW_AS_NEWLINE
            if(line->buffer[i + offset->xOffset] == '\n')
                mvwprintw(win->window, y, i, "¶");
            else
            #endif
                mvwprintw(win->window, y, i, "%c", line->buffer[i + offset->xOffset]);
    }
    */
    wmove(win, y, x);
    waddnstr(win, &line->buffer[x + offset->xOffset], n);
#if DISPLAY_PILCROW_AS_NEWLINE
    if(line->buffer[line->size - 1] == '\n')
        waddstr(win, "¶");
#endif
}

void display_scroll(WINDOW *win, enum scroll_direction direction)
{
    scrollok(win, TRUE); 
    wscrl(win, (direction == FORWARD) ? 1 : -1);
    scrollok(win, FALSE);

    if(direction == FORWARD)
    {
        display_blankRow(win, WINDOW_HEIGHT(win), 0, WINDOW_WIDTH(win));
    }

    else
    {
        display_blankRow(win, 0, 0, WINDOW_WIDTH(win)); 
    }
}
