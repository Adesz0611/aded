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

void display_buffer(ADED_WINDOW *win, line_t *line, int y, int nrow)
{
    int i;
    line_t *tmp = line;
    
    for(i = y; i < y + nrow; i++)
    {
        display_blankRow(win, i, 0, win->width);
    }

    for(i = y; i < y + nrow && tmp != NULL; i++, tmp = tmp->next)
    {
        display_line(win, tmp, i, 0, win->width);
    }
}

/*-- Blank--*/

/*  This function puts n spaces to the terminal */
void display_blankRow(ADED_WINDOW *win, int y, int x, int n)
{
    wmove(win->window, y, x);

    for(; n > 0; n--)
        waddch(win->window, ' ');
}

/*-- Draw --*/

/* Display n characters of a line*/
void display_line(ADED_WINDOW *win, line_t *line, int y, int x, int n)
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
    wmove(win->window, y, x);
    waddnstr(win->window, &line->buffer[x + offset->xOffset], n);
#if DISPLAY_PILCROW_AS_NEWLINE
    if(line->buffer[line->size - 1] == '\n')
        waddstr(win->window, "¶");
#endif
}

void display_scroll(ADED_WINDOW *win, enum scroll_direction direction)
{
    scrollok(win->window, TRUE); 
    wscrl(win->window, (direction == FORWARD) ? 1 : -1);
    scrollok(win->window, FALSE);

    if(direction == FORWARD)
    {
        display_blankRow(win, win->height, 0, win->width);
    }

    else
    {
        display_blankRow(win, 0, 0, win->width); 
    }
}
