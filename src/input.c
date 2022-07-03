/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "curses.h"
#include "input.h"
#include "buffer.h"
#include "line.h"
#include "defs.h"
#include "file.h"
#include "cursor.h"
#include "types.h"
#include "display.h"
#include "utf8.h"

static wchar_t input_wchar;

static void enter(void);
#if ALLOW_HOME_AND_END_KEY
static void move_home(void);
static void move_end(void);
#endif // ALLOW_HOME_AND_END_KEY
static void move_up(void);
static void move_down(void);
static void move_left(void);
static void move_right(void);
static void tab(void);


void input(void)
{
    input_wchar = wgetch(main_window);
    switch(input_wchar)
    {
        // Ncurses' resize handler is better than SIGWINCH
        case KEY_RESIZE:
            curses_resize();
            break;
        case KEY_F(10):
            exit(EXIT_SUCCESS);
            break;
        case KEY_F(5):
            file_save(buffer);
            break;
        case ENTER:
            enter();
            break;
        case 127:
        case '\b':
        case KEY_BACKSPACE:
            if(buffer->cursX > 0)
            {
                //FIXME:
                while(!isutf8(buffer->line_current->buffer[--buffer->cursX])) {
                    memmove(&buffer->line_current->buffer[buffer->cursX], &buffer->line_current->buffer[buffer->cursX + 1], buffer->line_current->size - buffer->cursX);
                    buffer->line_current->buffer[buffer->line_current->size - 1] = '\0';
                    buffer->line_current->size--;
                }
                memmove(&buffer->line_current->buffer[buffer->cursX], &buffer->line_current->buffer[buffer->cursX + 1], buffer->line_current->size - buffer->cursX);
                buffer->line_current->buffer[buffer->line_current->size - 1] = '\0';
                buffer->line_current->size--;

                buffer->cursRX--;

                /*TODO: Itt tartok */

                if(cursor->cursX < 1)
                {
                    if(buffer->cursRX < XSCROLL_VALUE)
                    {
                        buffer->xOffset = buffer->xROffset = 0;
                        //cursor->cursX += buffer->cursX;
                        cursor->cursX = buffer->cursRX;
                    }
                    else
                    {
                        buffer->xROffset -= XSCROLL_VALUE;
                        buffer->xOffset -= utf8_iterate_backward(buffer->line_current->buffer, buffer->cursX, XSCROLL_VALUE);
                        cursor->cursX += XSCROLL_VALUE;
                    }

                    full_redraw(main_window);
                }
                else
                {
                    if(buffer->line_current->size - 1 - buffer->xOffset > (size_t)WINDOW_WIDTH(main_window))
                    {
                        display_blankRow(main_window, cursor->cursY, cursor->cursX - 1, WINDOW_WIDTH(main_window) - cursor->cursX + 1);
                        display_line(main_window, buffer->line_current, cursor->cursY, cursor->cursX - 1, WINDOW_WIDTH(main_window) - cursor->cursX + 1);
                    }
                    else
                    {
                        display_blankRow(main_window, cursor->cursY, cursor->cursX - 1, buffer->line_current->size - buffer->cursX);
                        display_line(main_window, buffer->line_current, cursor->cursY, cursor->cursX - 1, buffer->line_current->size - buffer->cursX);
                    }
                }

                cursor->cursX--;
                buffer->cursX--;
                buffer->cursXsh = buffer->cursX;
                buffer->line_current->size--;
            }
            else
            {
                if(buffer->line_current->prev != buffer->line_head)
                { 
                    line_delete(BY_BACKSPACE, buffer);
                    display_buffer(main_window, buffer->line_current, cursor->cursY, WINDOW_HEIGHT(main_window) - cursor->cursY);
                }
            }
            break;

        case KEY_DC: // Delete key
            if(buffer->cursX < buffer->line_current->size - 1)
            {
                memmove(&buffer->line_current->buffer[buffer->cursX], &buffer->line_current->buffer[buffer->cursX + 1], buffer->line_current->size - buffer->cursX);
                buffer->line_current->buffer[buffer->line_current->size + 1] = '\0';
                buffer->line_current->size--;

                // TODO: Put it into a function
                // If statements for drawing
                if(buffer->line_current->size - 1 - buffer->xOffset > (size_t)WINDOW_WIDTH(main_window))
                {
                    display_blankRow(main_window, cursor->cursY, cursor->cursX, WINDOW_WIDTH(main_window) - cursor->cursX);
                    display_line(main_window, buffer->line_current, cursor->cursY, cursor->cursX, WINDOW_WIDTH(main_window) - cursor->cursX);
                }
                else
                {
                    display_blankRow(main_window, cursor->cursY, cursor->cursX, buffer->line_current->size - buffer->cursX);
                    display_line(main_window, buffer->line_current, cursor->cursY, cursor->cursX, buffer->line_current->size - buffer->cursX);
                }
            }
            else if(buffer->line_current->next != NULL)
            {
                line_delete(BY_DELETE, buffer);
                
                // If statements for drawing
                if(buffer->line_current->size - 1 - buffer->xOffset > (size_t)WINDOW_WIDTH(main_window))
                {
                    display_blankRow(main_window, cursor->cursY, cursor->cursX, WINDOW_WIDTH(main_window) - cursor->cursX);
                    display_line(main_window, buffer->line_current, cursor->cursY, cursor->cursX, WINDOW_WIDTH(main_window) - cursor->cursX);
                }
                else
                {
                    display_blankRow(main_window, cursor->cursY, cursor->cursX, buffer->line_current->size - 1 - buffer->cursX);
                    display_line(main_window, buffer->line_current, cursor->cursY, cursor->cursX, buffer->line_current->size - 1 - buffer->cursX);
                }
                display_buffer(main_window, buffer->line_current, cursor->cursY, WINDOW_HEIGHT(main_window) - cursor->cursY);
            }
            break;
        case KEY_NPAGE:
            display_page_down(main_window);
            break;
        case KEY_PPAGE:
            display_page_up(main_window);
            break;
        case KEY_TAB:
            tab();
            break;
#if ALLOW_HOME_AND_END_KEY
        case KEY_HOME:
            move_home();
            break;
        case KEY_END:
            move_end();
            break;
#endif //ALLOW_HOME_AND_END_KEY
        case KEY_UP:
            move_up();
            break;
        case KEY_DOWN:
            move_down();
            break;
        case KEY_LEFT:
            move_left();
            break;
        case KEY_RIGHT:
            move_right();
            break;
        default:
            memmove(&buffer->line_current->buffer[buffer->cursX + 1], &buffer->line_current->buffer[buffer->cursX], buffer->line_current->size - buffer->cursX);
            buffer->line_current->buffer[buffer->cursX] = input_wchar;
            buffer->cursX++;
            buffer->line_current->size++;

            // TODO: Gondolkodni jobb megoldáson
            // Függvénybe rakni a keyboard_inputot
            if(!isascii(input_wchar)) {
            nodelay(main_window, TRUE);
            // Read the remaining characters
            while(1)
            {
                input_wchar = wgetch(main_window);
                if(input_wchar == ERR)
                    break;
                memmove(&buffer->line_current->buffer[buffer->cursX + 1], &buffer->line_current->buffer[buffer->cursX], buffer->line_current->size - buffer->cursX);
                buffer->line_current->buffer[buffer->cursX] = input_wchar;
                buffer->line_current->size++;
                buffer->cursX++;

                if(isutf8(input_wchar))
                    cursor->cursX++;
            }
            nodelay(main_window, FALSE);
            }

            if(cursor->cursX > WINDOW_WIDTH(main_window) - 2)
            {
                buffer->xOffset += XSCROLL_VALUE;
                cursor->cursX -= XSCROLL_VALUE - 1;
                display_buffer(main_window, buffer->line_yOffset, 0, WINDOW_HEIGHT(main_window));
            }
            else
            {
            /*
                // If statements for drawing
                if(buffer->line_current->size - 1 - buffer->xOffset > (size_t)WINDOW_WIDTH(main_window))
                {
                    display_blankRow(main_window, cursor->cursY, cursor->cursX, WINDOW_WIDTH(main_window) - cursor->cursX);
                    display_line(main_window, buffer->line_current, cursor->cursY, cursor->cursX, WINDOW_WIDTH(main_window) - cursor->cursX);
                }
                else
                {
                    display_blankRow(main_window, cursor->cursY, cursor->cursX, buffer->line_current->size - 1 - buffer->cursX);
                    display_line(main_window, buffer->line_current, cursor->cursY, cursor->cursX, buffer->line_current->size - 1 - buffer->cursX);
                }
                
                cursor->cursX++;
            */
                display_blankRow(main_window, cursor->cursY, 0, WINDOW_WIDTH(main_window));
                display_line(main_window, buffer->line_current, cursor->cursY, 0, buffer->line_current->size);
                cursor->cursX++;
            }
            buffer->cursXsh = buffer->cursX;
            buffer->cursRX++;
            break;
    }
}

static void enter(void)
{
    line_add("", buffer);
    strncpy(&buffer->line_current->buffer[0], &buffer->line_current->prev->buffer[buffer->cursX], buffer->line_current->prev->size - 1 - buffer->cursX); // '\n' is not element of size!!!
    
    // Delete all of bytes after cursor (also '\n')
    memset(&buffer->line_current->prev->buffer[buffer->cursX], 0, buffer->line_current->prev->size - buffer->cursX);
    buffer->line_current->prev->buffer[buffer->cursX] = '\n';

    buffer->line_current->size += buffer->line_current->prev->size - 1 - buffer->cursX;
    buffer->line_current->buffer[buffer->line_current->size - 1] = '\n';
    

    if(cursor->cursY > WINDOW_HEIGHT(main_window) - 2)
    {
        buffer->line_yOffset = buffer->line_yOffset->next;
        display_blankRow(main_window, cursor->cursY, cursor->cursX, (buffer->line_current->prev->size - 1 - buffer->xOffset > (size_t)WINDOW_WIDTH(main_window)) ? WINDOW_WIDTH(main_window) - cursor->cursX : buffer->line_current->prev->size - 1 - buffer->cursX);

        display_scroll(main_window, FORWARD);

        display_line(main_window, buffer->line_current, WINDOW_HEIGHT(main_window) - 1, 0, WINDOW_WIDTH(main_window)); // Display it, otherwise not
    }
    else
    {
        cursor->cursY++;

        display_blankRow(main_window, cursor->cursY - 1, cursor->cursX, (buffer->line_current->prev->size - 1 - buffer->xOffset > (size_t)WINDOW_WIDTH(main_window)) ? WINDOW_WIDTH(main_window) - cursor->cursX : buffer->line_current->prev->size - 1 - buffer->cursX);

        if(buffer->line_current != buffer->line_tail)
        {
            display_buffer(main_window, buffer->line_current, cursor->cursY, WINDOW_HEIGHT(main_window) - cursor->cursY);
        }
        else
        {
            display_line(main_window, buffer->line_current, cursor->cursY, 0, WINDOW_HEIGHT(main_window));
        }
    }

    buffer->line_current->prev->size -= buffer->line_current->size - 1;

    bool shouldRedraw = (buffer->xOffset) ? true : false;
    buffer->cursY++;
    buffer->xOffset = 0;

    if(shouldRedraw)
        display_buffer(main_window, buffer->line_yOffset, 0, WINDOW_HEIGHT(main_window));

    cursor->cursX = 0;
    buffer->cursX = 0;
    buffer->cursXsh = 0;
}

#if ALLOW_HOME_AND_END_KEY
static void move_home(void)
{
    bool shouldRedraw = (buffer->xOffset) ? true : false;
    cursor->cursX = 0;
    buffer->cursX = 0;
    buffer->cursXsh = 0;
    buffer->xOffset = 0;

    if(shouldRedraw)
        display_buffer(main_window, buffer->line_yOffset, 0, WINDOW_HEIGHT(main_window));
}

static void move_end(void)
{
    buffer->cursX = buffer->line_current->size - 1;
    buffer->cursXsh = buffer->cursX;

    if(buffer->line_current->size - 1 - buffer->xOffset >= (size_t)WINDOW_WIDTH(main_window))
    {
        cursor->cursX = WINDOW_WIDTH(main_window) - XSCROLL_VALUE;
        buffer->xOffset = buffer->cursX - WINDOW_WIDTH(main_window) + XSCROLL_VALUE;

        display_buffer(main_window, buffer->line_yOffset, 0, WINDOW_HEIGHT(main_window));
    }
    else
        cursor->cursX = buffer->line_current->size - 1 - buffer->xOffset;
}
#endif // ALLOW_HOME_AND_END_KEY

static void move_up(void)
{
    if(buffer->line_current->prev != buffer->line_head)
    {
        if(cursor->cursY < getbegy(main_window) + 1)
        {
            buffer->line_yOffset = buffer->line_yOffset->prev;
            display_scroll(main_window, BACKWARD);
            display_line(main_window, buffer->line_yOffset, 0, 0, WINDOW_WIDTH(main_window)); // Display it, otherwise not
        }
        else
            cursor->cursY--;

        buffer->line_current = buffer->line_current->prev;
        buffer->cursY--;
       
        display_position_cursor_horizontally(main_window, buffer, true);
    }
}

// #define move_left move_any(1, 0, 0, 0)
// #define move_up move_any(0, 1, 0, 0)

static void move_down(void)
{
    if(buffer->line_current->next != NULL)
    {
        if(cursor->cursY > WINDOW_HEIGHT(main_window) - 2)
        {
            buffer->line_yOffset = buffer->line_yOffset->next;
            display_scroll(main_window, FORWARD);
            display_line(main_window, buffer->line_current->next, WINDOW_HEIGHT(main_window) - 1, 0, WINDOW_WIDTH(main_window)); // Display it, otherwise not
        }
        else
            cursor->cursY++;

        buffer->line_current = buffer->line_current->next;
        buffer->cursY++;

        display_position_cursor_horizontally(main_window, buffer, true);
    }
}

static void move_left(void)
{
    if(buffer->cursX > 0)
    {
        if(cursor->cursX < 1)
        {
            if(buffer->cursX < XSCROLL_VALUE)
            {
                buffer->xOffset -= buffer->cursX;
                cursor->cursX += buffer->cursX - 1;
            }
            else
            {
                buffer->xOffset -= XSCROLL_VALUE;
                cursor->cursX += XSCROLL_VALUE - 1;
            }
            full_redraw(main_window);
        }

        else
            cursor->cursX--;

        buffer->cursX--;
        buffer->cursXsh = buffer->cursX;
    }
    else
    {
        if(buffer->line_current->prev != buffer->line_head)
        {
            buffer->line_current = buffer->line_current->prev;
            
            if(cursor->cursY < 1)
            {
                if(buffer->line_yOffset->prev != buffer->line_head)
                    buffer->line_yOffset = buffer->line_yOffset->prev;

                display_scroll(main_window, BACKWARD);
                display_line(main_window, buffer->line_yOffset, 0, 0, WINDOW_WIDTH(main_window));
            }

            else
                cursor->cursY--;
            
            buffer->cursY--;

            buffer->cursX = buffer->line_current->size - 1;
            buffer->cursXsh = buffer->cursX;
            if((int)buffer->line_current->size - 1 >= WINDOW_WIDTH(main_window))
            {
                cursor->cursX = WINDOW_WIDTH(main_window) - XSCROLL_VALUE;
                buffer->xOffset = buffer->cursX - WINDOW_WIDTH(main_window) + XSCROLL_VALUE; 
                full_redraw(main_window);
            }
            else
                cursor->cursX = buffer->line_current->size - 1;
        }
    }
}

static void move_right(void)
{
    if(buffer->cursX < buffer->line_current->size - 1)
    {
        if (cursor->cursX > WINDOW_WIDTH(main_window) - 2)
        {
            buffer->xOffset += XSCROLL_VALUE;
            cursor->cursX -= XSCROLL_VALUE - 1;
            full_redraw(main_window);
        }
        else
            cursor->cursX++;
        
        buffer->cursX++;
        buffer->cursXsh = buffer->cursX;
    }

    else
    {
        if(buffer->line_current->next != NULL)
        {
            buffer->line_current = buffer->line_current->next;

            if(cursor->cursY > WINDOW_HEIGHT(main_window) - 2)
            {
                buffer->line_yOffset = buffer->line_yOffset->next;

                display_scroll(main_window, FORWARD);
                display_line(main_window, buffer->line_current, WINDOW_HEIGHT(main_window) - 1, 0, WINDOW_WIDTH(main_window));
            }

            else
                cursor->cursY++;
            
            buffer->cursY++;

            if(buffer->xOffset != 0)
            {
                buffer->xOffset = 0;
                full_redraw(main_window);
            }

            cursor->cursX = 0;
            buffer->cursX = 0;
            buffer->cursXsh = 0;
            buffer->xOffset = 0;
        }
    }
}

static void tab(void)
{
    u8 tmp_tabsize = buffer->cursX % TAB_WIDTH;
    tmp_tabsize = TAB_WIDTH - tmp_tabsize;

    memmove(&buffer->line_current->buffer[buffer->cursX + tmp_tabsize], &buffer->line_current->buffer[buffer->cursX], buffer->line_current->size - buffer->cursX); // Must memmove because of '\n' character
    for(size_t i = buffer->cursX; i < buffer->cursX + tmp_tabsize; i++)
        buffer->line_current->buffer[i] = ' ';
    

    buffer->line_current->size += tmp_tabsize;

    if (cursor->cursX + tmp_tabsize >= WINDOW_WIDTH(main_window))
    {
        buffer->xOffset += XSCROLL_VALUE + tmp_tabsize;
        cursor->cursX -= XSCROLL_VALUE;

        full_redraw(main_window);
    }
    else
    {

        // If statements for drawing
        if(buffer->line_current->size - 1 - buffer->xOffset > (size_t)WINDOW_WIDTH(main_window))
        {
            display_blankRow(main_window, cursor->cursY, cursor->cursX, WINDOW_WIDTH(main_window) - cursor->cursX);
            display_line(main_window, buffer->line_current, cursor->cursY, cursor->cursX, WINDOW_WIDTH(main_window) - cursor->cursX);
        }
        else
        {
            display_blankRow(main_window, cursor->cursY, cursor->cursX, buffer->line_current->size - 1 - buffer->cursX);
            display_line(main_window, buffer->line_current, cursor->cursY, cursor->cursX, buffer->line_current->size - 1 - buffer->cursX);
        }

        cursor->cursX += tmp_tabsize;
    }

    buffer->cursX += tmp_tabsize;
    buffer->cursXsh = buffer->cursX;
}
