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

static wchar_t input_wchar;

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
    input_wchar = wgetch(main_window->window);
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
            file_save();
            break;
        case ENTER:
            line_add("");
            strncpy(&line_current->buffer[0], &line_current->prev->buffer[buffer->cursX], line_current->prev->size - 1 - buffer->cursX); // '\n' is not element of size!!!
            
            // Delete all of bytes after cursor (also '\n')
            memset(&line_current->prev->buffer[buffer->cursX], 0, line_current->prev->size - buffer->cursX);
            line_current->prev->buffer[buffer->cursX] = '\n';

            line_current->size += line_current->prev->size - 1 - buffer->cursX;
            line_current->prev->size -= line_current->size - 1;
            line_current->buffer[line_current->size - 1] = '\n';
            
            if(cursor->cursY > main_window->height - 2)
                offset->line_yOffset = offset->line_yOffset->next;
            else
                cursor->cursY++;
            buffer->cursY++;

            cursor->cursX = 0;
            buffer->cursX = 0;
            offset->xOffset = 0;
            break;
        case 127:
        case '\b':

        // FIXME:
        case KEY_BACKSPACE:
            if(buffer->cursX > 0)
            {
                memmove(&line_current->buffer[buffer->cursX - 1], &line_current->buffer[buffer->cursX], line_current->size + 1 - buffer->cursX);
                line_current->buffer[line_current->size] = '\0';

                if(cursor->cursX < 1)
                {
                    offset->xOffset -= XSCROLL_VALUE;
                    cursor->cursX += XSCROLL_VALUE;
                }

                cursor->cursX--;
                buffer->cursX--;
                line_current->size--;
            }
            else
            {
                if(line_current->prev != line_head)
                { 
                    line_delete(BY_BACKSPACE);
                }
            }
            break;

        // FIXME:
        case KEY_DC: // Delete key
            if(buffer->cursX < (int)line_current->size - 1)
            {
                memmove(&line_current->buffer[buffer->cursX], &line_current->buffer[buffer->cursX + 1], line_current->size - buffer->cursX);
                line_current->buffer[line_current->size + 1] = '\0';
                line_current->size--;
            }
            else if(line_current->next != NULL)
            {
                line_delete(BY_DELETE);
            }
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
            if(isascii(input_wchar))
            {
                memmove(&line_current->buffer[buffer->cursX + 1], &line_current->buffer[buffer->cursX], line_current->size - buffer->cursX);
                line_current->buffer[buffer->cursX] = input_wchar;

                if(cursor->cursX > main_window->width - 2)
                {
                    offset->xOffset += XSCROLL_VALUE;
                    cursor->cursX -= XSCROLL_VALUE - 1;
                }
                else
                    cursor->cursX++;
                line_current->size++;
                buffer->cursX++;
            }
            break;
    }
}

#if ALLOW_HOME_AND_END_KEY
static void move_home(void)
{
    cursor->cursX = 0;
    buffer->cursX = 0;
    offset->xOffset = 0;
}

static void move_end(void)
{
    if((int)line_current->size - 1 > termInfo->width)
    {
        /*
        cursor->cursX = termInfo->width - 1;
        offset->xOffset = line_current->size - termInfo->width;
        */
        cursor->cursX = main_window->width - XSCROLL_VALUE;
        offset->xOffset = line_current->size - XSCROLL_VALUE - 1;
    }
    else
        cursor->cursX = line_current->size - 1;

    buffer->cursX = line_current->size - 1;
}
#endif // ALLOW_HOME_AND_END_KEY

static void move_up(void)
{
    if(line_current->prev != line_head)
    {
        if(cursor->cursY < 1)
            offset->line_yOffset = offset->line_yOffset->prev;
        else
            cursor->cursY--;

        buffer->cursY--;
        line_current = line_current->prev;
        
        // TODO: Put it into a function
        if(buffer->cursX > (int)line_current->size - 1)
        {
            buffer->cursX = line_current->size - 1;
            cursor->cursX = (line_current->size - 1) % termInfo->width;
        }
    }
}

// #define move_left move_any(1, 0, 0, 0)
// #define move_up move_any(0, 1, 0, 0)

static void move_down(void)
{
    if(line_current->next != NULL)
    {
        if(cursor->cursY > main_window->height - 2)
            offset->line_yOffset = offset->line_yOffset->next;
        else
            cursor->cursY++;

        buffer->cursY++;
        line_current = line_current->next;
        if(cursor->cursX > (int)line_current->size - 1)
        {
            buffer->cursX = line_current->size - 1;
            cursor->cursX = (line_current->size - 1) % termInfo->width;
        }
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
                offset->xOffset -= buffer->cursX;
                cursor->cursX += buffer->cursX - 1;
            }
            else
            {
                offset->xOffset -= XSCROLL_VALUE;
                cursor->cursX += XSCROLL_VALUE - 1;
            }
        }

        else
            cursor->cursX--;

        buffer->cursX--;
    }
    else
    {
        if(line_current->prev != line_head)
        {
            line_current = line_current->prev;
            
            if(cursor->cursY < 1)
            {
                offset->line_yOffset = offset->line_yOffset->prev;
                
            }

            else
            {
                cursor->cursY--;
                buffer->cursY--;
            }

            buffer->cursX = line_current->size - 1;
            cursor->cursX = (line_current->size - 1) % termInfo->width;
        }
    }
}

static void move_right(void)
{
    if(buffer->cursX < (int)line_current->size - 1)
    {
        if (cursor->cursX > termInfo->width - 2)
        {
            offset->xOffset += XSCROLL_VALUE;
            cursor->cursX -= XSCROLL_VALUE - 1;
        }
        else
            cursor->cursX++;
        
        buffer->cursX++;
    }

    else
    {
        if(line_current->next != NULL)
        {
            line_current = line_current->next;

            if(cursor->cursY > main_window->height - 2)
            {
                offset->line_yOffset = offset->line_yOffset->next;
            }

            else
            {
                cursor->cursY++;
                buffer->cursY++;
            }
            
            cursor->cursX = 0;
            buffer->cursX = 0;
            offset->xOffset = 0;
        }
    }
}

static void tab(void)
{
    u8 tmp_tabsize = buffer->cursX % TAB_WIDTH;
    tmp_tabsize = TAB_WIDTH - tmp_tabsize;

    memmove(&line_current->buffer[buffer->cursX + tmp_tabsize], &line_current->buffer[buffer->cursX], line_current->size - buffer->cursX); // Must memmove because of '\n' character
    for(int i = buffer->cursX; i < buffer->cursX + tmp_tabsize; i++)
        line_current->buffer[i] = ' ';
    
    if (cursor->cursX > termInfo->width - 2)
    {
        offset->xOffset += XSCROLL_VALUE + tmp_tabsize;
        cursor->cursX -= XSCROLL_VALUE;
    }
    else
    {
        cursor->cursX += tmp_tabsize;
    }

    buffer->cursX += tmp_tabsize;
    line_current->size += tmp_tabsize;
}
