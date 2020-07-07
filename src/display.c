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
        display_blankRow(win, i, win->width, win->width);
    }

    for(i = y; i < y + nrow && tmp != NULL; i++, tmp = tmp->next)
    {
        display_line(win, tmp, i);
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

// TODO:    implement a display_nline(ADED_WINDOW *win, line_t *line, int y, int x, int n) funtion then
//          define a macro for display_line(ADED_WINDOW *win, line_t *line, int y)
/* Display n characters of a line*/
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
    
        // If the next line is exist
        if(line_current->next != NULL)
            display_line(main_window, line_current->next, main_window->height - 1); // Display it, otherwise not
    }

    else
    {
        display_blankRow(win, 0, win->width, win->width);
    
        // If the yOffset line's previous is exist
        if(offset->line_yOffset->prev != line_head)
            display_line(main_window, offset->line_yOffset->prev, 0); // Display it, otherwise not
    }
}
