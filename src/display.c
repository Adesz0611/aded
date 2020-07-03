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

void display_buffer(ADED_WINDOW *win)
{
    int i;
    line_t *tmp = offset->line_yOffset;
    
    for(i = 0; i < win->height; i++)
    {
        display_blankRow(win, i, 0, win->width);
    }

    for(i = 0; i < win->height && tmp != NULL; i++, tmp = tmp->next)
    {
        display_line(win, tmp, i);
    }
}

//-- Blank --//

/*
 *  This function t
 */
void display_blankRow(ADED_WINDOW *win, int y, int x, int n)
{
    wmove(win->window, y, x);

    for(; n > 0; n--)
        waddch(win->window, ' ');
}

//-- Draw --//
void display_line(ADED_WINDOW *win, line_t *line, int row)
{
    for(int i = 0; i < win->width && line->buffer[i] != '\0'; i++)
    {
            #if DISPLAY_PILCROW_AS_NEWLINE
            if(line->buffer[i + offset->xOffset] == '\n')
                mvwprintw(win->window, row, i, "¶");
            else
            #endif
                mvwprintw(win->window, row, i, "%c", line->buffer[i + offset->xOffset]);
    }
}

void display_scroll(ADED_WINDOW *win, enum scroll_direction direction)
{
    scrollok(win->window, TRUE); 
    wscrl(win->window, (direction == FORWARD) ? 1 : -1);
    scrollok(win->window, FALSE);

    if(direction == FORWARD)
    {
        display_blankRow(win, win->height, win->width, win->width);
    }
}
