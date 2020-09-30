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
#include "buffer.h"
#include "cursor.h"

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
    // TODO:
/*
    for(int i = 0; i < n && line->buffer[i] != NULL; i++)
    {

    }
*/
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

void display_onepage(WINDOW *win, enum scroll_direction direction)
{
    if(direction == FORWARD)
    {
        line_t *tmp;
        int i = 0;
        for(i = 0, tmp = line_tail; i < WINDOW_HEIGHT(win) - 1 && tmp != NULL; i++, tmp = tmp->prev);
        if(tmp != offset->line_yOffset)
        {
            for(int i = 0; i < ONEPAGE_VALUE && offset->line_yOffset != tmp && line_current != line_tail; i++)
            {
                offset->line_yOffset = offset->line_yOffset->next;
                line_current = line_current->next;
                buffer->cursY++;

            }

            display_position_cursor_horizontally(main_window, buffer, false);
            full_redraw(win);
        }
        else
        {
            buffer->cursY = buffer->numlines - 1;
            cursor->cursY = WINDOW_HEIGHT(win) - 1;
            line_current = line_tail;
        }
    }
    else
    {
        // First check we should display a page up or not
        // If we are in the beginning of the buffer we don't have to
        if(buffer->cursY > 0)
        {
            if(offset->line_yOffset != line_head->next)
            {
                for(int i = 0; i < ONEPAGE_VALUE && offset->line_yOffset != line_head->next && line_current != line_head->next; i++)
                {
                    offset->line_yOffset = offset->line_yOffset->prev;
                    line_current = line_current->prev;
                    buffer->cursY--;
                }

                display_position_cursor_horizontally(main_window, buffer, false);

                full_redraw(win);
            }
            else
            {
                cursor->cursY = 0;
                buffer->cursY = 0;
                line_current = line_head->next;
            }
        }
    }
}

// position the cursor horizantally and if needed redraw the screen
void display_position_cursor_horizontally(WINDOW *win, buffer_t *p_buffer, bool shouldRedraw)
{
    if(line_current->size - 1 < p_buffer->cursX)
    {
        p_buffer->cursX = line_current->size - 1;

        if(WINDOW_WIDTH(win) < line_current->size - 1)
        {
            if(offset->xOffset < line_current->size - 1)
                cursor->cursX = line_current->size - 1 - offset->xOffset;
            else
            {
                cursor->cursX = WINDOW_WIDTH(win) - XSCROLL_VALUE;
                offset->xOffset = p_buffer->cursX - WINDOW_WIDTH(win) + XSCROLL_VALUE; //You can use line_current->size - 1 instead of buffer->cursX
                if(shouldRedraw)
                    full_redraw(win);
            }
        }
        else
        {
            cursor->cursX = line_current->size - 1;
            offset->xOffset = 0;
            if(shouldRedraw)
                full_redraw(win);
        }
    }
}
