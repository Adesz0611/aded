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
    input_wchar = wgetch(curses->window);
    switch(input_wchar)
    {
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
            
            if(buffer->cursY > curses->termY - 2)
                line_yOffset = line_yOffset->next;
            else
                buffer->cursY++;
            buffer->cursX = 0;
            break;
        case 127:
        case '\b':
        case KEY_BACKSPACE:
            if(buffer->cursX != 0)
            {
                memmove(&line_current->buffer[buffer->cursX - 1], &line_current->buffer[buffer->cursX], line_current->size + 1 - buffer->cursX);
                line_current->buffer[line_current->size] = '\0';
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
        case KEY_DC: // Delete key
            if(line_current->size > 1 && buffer->cursX != (int)line_current->size - 1)
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
    
                line_current->size++;
                buffer->cursX++;
            }
            break;
    }
}

#if ALLOW_HOME_AND_END_KEY
static void move_home(void)
{
    buffer->cursX = 0;
}

static void move_end(void)
{
    buffer->cursX = (int)line_current->size - 1;
}
#endif // ALLOW_HOME_AND_END_KEY

static void move_up(void)
{
    if(line_current->prev != line_head)
    {
        if(buffer->cursY < 1)
            line_yOffset = line_yOffset->prev;
        else
            buffer->cursY--;
        line_current = line_current->prev;
        if(buffer->cursX > (int)line_current->size - 1)
        {
            buffer->cursX = line_current->size - 1;
        }
    }
}

// #define move_left move_any(1, 0, 0, 0)
// #define move_up move_any(0, 1, 0, 0)

static void move_down(void)
{
    if(line_current->next != NULL)
    {
        if(buffer->cursY > curses->termY - 2)
            line_yOffset = line_yOffset->next;
        else
            buffer->cursY++;
        line_current = line_current->next;
        if(buffer->cursX > (int)line_current->size - 1)
        {
            buffer->cursX = line_current->size - 1;
        }
    }
}

static void move_left(void)
{
    if(buffer->cursX > 0)
        buffer->cursX--;
    else
    {
        if(line_current->prev != line_head)
        {
            line_current = line_current->prev;
            buffer->cursY--;
            buffer->cursX = line_current->size - 1;
        }
    }
}

static void move_right(void)
{
    if(buffer->cursX < (int)line_current->size - 1)
        buffer->cursX++;
    else
    {
        if(line_current->next != NULL)
        {
            line_current = line_current->next;
            buffer->cursY++;
            buffer->cursX = 0;
        }
    }
}

static void tab(void)
{
    memmove(&line_current->buffer[buffer->cursX + TAB_WIDTH], &line_current->buffer[buffer->cursX], line_current->size - buffer->cursX); // Must memmove because of '\n' character
    for(int i = buffer->cursX; i < buffer->cursX + TAB_WIDTH; i++)
        line_current->buffer[i] = ' ';
    buffer->cursX += TAB_WIDTH;
    line_current->size += TAB_WIDTH;
}
