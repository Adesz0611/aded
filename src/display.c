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
    waddnstr(win, &line->buffer[x + buffer->xOffset], n);
#if DISPLAY_PILCROW_AS_NEWLINE
    if(buffer->line->buffer[buffer->line->size - 1] == '\n')
        waddstr(win, "¶");
#endif
    // TODO:
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
        for(i = 0, tmp = buffer->line_tail; i < WINDOW_HEIGHT(win) - 1 && tmp != NULL; i++, tmp = tmp->prev);
        if(tmp != buffer->line_yOffset)
        {
            for(int i = 0; i < ONEPAGE_VALUE && buffer->line_yOffset != tmp && buffer->line_current != buffer->line_tail; i++)
            {
                buffer->line_yOffset = buffer->line_yOffset->next;
                buffer->line_current = buffer->line_current->next;
                buffer->cursY++;

            }

            display_position_cursor_horizontally(main_window, buffer, false);
            full_redraw(win);
        }
        else
        {
            buffer->cursY = buffer->numlines - 1;
            cursor->cursY = WINDOW_HEIGHT(win) - 1;
            buffer->line_current = buffer->line_tail;
            display_position_cursor_horizontally(main_window, buffer, true);
        }
    }
    else
    {
        // First check we should display a page up or not
        // If we are in the beginning of the buffer we don't have to
        if(buffer->cursY > 0)
        {
            if(buffer->line_yOffset != buffer->line_head->next)
            {
                for(int i = 0; i < ONEPAGE_VALUE && buffer->line_yOffset != buffer->line_head->next && buffer->line_current != buffer->line_head->next; i++)
                {
                    buffer->line_yOffset = buffer->line_yOffset->prev;
                    buffer->line_current = buffer->line_current->prev;
                    buffer->cursY--;
                }

                display_position_cursor_horizontally(main_window, buffer, false);

                full_redraw(win);
            }
            else
            {
                cursor->cursY = 0;
                buffer->cursY = 0;
                buffer->line_current = buffer->line_head->next;
                display_position_cursor_horizontally(main_window, buffer, true);
            }
        }
    }
}

// position the cursor horizantally and if needed redraw the screen
void display_position_cursor_horizontally(WINDOW *win, buffer_t *p_buffer, bool shouldRedraw)
{
    if(p_buffer->line_current->size - 1 < p_buffer->cursXsh)
    {
        p_buffer->cursX = p_buffer->line_current->size - 1;

        if(WINDOW_WIDTH(win) + p_buffer->xOffset <= p_buffer->cursX)
        {
            cursor->cursX = WINDOW_WIDTH(win) - XSCROLL_VALUE;
            p_buffer->xOffset = p_buffer->cursX - WINDOW_WIDTH(win) + XSCROLL_VALUE;
            if(shouldRedraw)
                full_redraw(win);
        }
        else if(p_buffer->cursX < p_buffer->xOffset)
        {
            if((size_t)WINDOW_WIDTH(win) < p_buffer->cursX)
            {
                cursor->cursX = WINDOW_WIDTH(win) - XSCROLL_VALUE;
                p_buffer->xOffset = p_buffer->cursX - WINDOW_WIDTH(win) + XSCROLL_VALUE;
            }
            else
            {
                cursor->cursX = p_buffer->cursX;
                p_buffer->xOffset = 0;
            }

            if(shouldRedraw)
                full_redraw(win);
        }
        else
            cursor->cursX = p_buffer->cursX - p_buffer->xOffset;
    }
    else
    {
        if(WINDOW_WIDTH(win) + p_buffer->xOffset <= p_buffer->cursXsh)
        {
            p_buffer->cursX = p_buffer->cursXsh;

            cursor->cursX = WINDOW_WIDTH(win) - XSCROLL_VALUE;
            p_buffer->xOffset = p_buffer->cursX - WINDOW_WIDTH(win) + XSCROLL_VALUE;
            if(shouldRedraw)
                full_redraw(win);
        }
        else
        {
            p_buffer->cursX = p_buffer->cursXsh;
            cursor->cursX = p_buffer->cursXsh - p_buffer->xOffset;
        }
    }
}
