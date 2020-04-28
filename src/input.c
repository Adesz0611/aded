#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "curses.h"
#include "input.h"
#include "buffer.h"
#include "line.h"
#include "defs.h"
#include "file.h"

static wchar_t input_wchar;

static void move_home(void);
static void move_end(void);
static void move_up(void);
static void move_down(void);
static void move_left(void);
static void move_right(void);

void input(void)
{
    input_wchar = getch();
    switch(input_wchar)
    {
        case KEY_F(10):
            exit(EXIT_SUCCESS);
            break;
        case KEY_F(5):
            file_save();
            break;
        case ENTER:
            line_current->buffer[buffer->cursX] = '\n';
            line_add("");
            buffer->cursY++;
            buffer->cursX = 0;
            break;
        case KEY_BACKSPACE:
            if(buffer->cursX != 0)
            {
                buffer->cursX--;
                memmove(&line_current->buffer[buffer->cursX], &line_current->buffer[buffer->cursX + 1], line_current->size - buffer->cursX);
                line_current->size--;
            }
            break;
        case KEY_DC:
            if(line_current->size > 0)
            {
                memmove(&line_current->buffer[buffer->cursX], &line_current->buffer[buffer->cursX + 1], line_current->size - buffer->cursX);
                line_current->size--;
            }
            break;
        case KEY_HOME:
            move_home();
            break;
        case KEY_END:
            move_end();
            break;
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
            if(buffer->cursX == (int)line_current->size)
                line_current->buffer[buffer->cursX] = input_wchar;
            else
            {
                memmove(&line_current->buffer[buffer->cursX + 1], &line_current->buffer[buffer->cursX], line_current->size - buffer->cursX);
                line_current->buffer[buffer->cursX] = input_wchar;
            }
            line_current->size++;
            buffer->cursX++;
            break;
    }
}

static void move_home(void)
{
    buffer->cursX = 0;
}

static void move_end(void)
{
    buffer->cursX = (int)line_current->size;
}

static void move_up(void)
{
    if(line_current->prev != line_head)
    {
        line_current = line_current->prev;
        buffer->cursY--;
        if(buffer->cursX > (int)line_current->size)
        {
            buffer->cursX = line_current->size;
        }
    }
}

static void move_down(void)
{
    if(line_current->next != NULL)
    {
        line_current = line_current->next;
        buffer->cursY++;
        if(buffer->cursX > (int)line_current->size)
        {
            buffer->cursX = line_current->size;
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
            buffer->cursX = line_current->size;
        }
    }
}

static void move_right(void)
{
    if(buffer->cursX < (int)line_current->size)
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
