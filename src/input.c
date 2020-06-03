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
            strncpy(&line_current->buffer[0], &line_current->prev->buffer[cursor->cursX], line_current->prev->size - 1 - cursor->cursX); // '\n' is not element of size!!!
            
            // Delete all of bytes after cursor (also '\n')
            memset(&line_current->prev->buffer[cursor->cursX], 0, line_current->prev->size - cursor->cursX);
            line_current->prev->buffer[cursor->cursX] = '\n';

            line_current->size += line_current->prev->size - 1 - cursor->cursX;
            line_current->prev->size -= line_current->size - 1;
            line_current->buffer[line_current->size - 1] = '\n';
            
            if(cursor->cursY > main_window->height - 2)
                line_yOffset = line_yOffset->next;
            else
                cursor->cursY++;
            cursor->cursX = 0;
            break;
        case 127:
        case '\b':
        case KEY_BACKSPACE:
            if(cursor->cursX != 0)
            {
                memmove(&line_current->buffer[cursor->cursX - 1], &line_current->buffer[cursor->cursX], line_current->size + 1 - cursor->cursX);
                line_current->buffer[line_current->size] = '\0';
                cursor->cursX--;
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
            if(line_current->size > 1 && cursor->cursX != (int)line_current->size - 1)
            {
                memmove(&line_current->buffer[cursor->cursX], &line_current->buffer[cursor->cursX + 1], line_current->size - cursor->cursX);
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
                memmove(&line_current->buffer[cursor->cursX + 1], &line_current->buffer[cursor->cursX], line_current->size - cursor->cursX);
                line_current->buffer[cursor->cursX] = input_wchar;
    
                line_current->size++;
                cursor->cursX++;
            }
            break;
    }
}

#if ALLOW_HOME_AND_END_KEY
static void move_home(void)
{
    cursor->cursX = 0;
}

static void move_end(void)
{
    cursor->cursX = (int)line_current->size - 1;
}
#endif // ALLOW_HOME_AND_END_KEY

static void move_up(void)
{
    if(line_current->prev != line_head)
    {
        if(cursor->cursY < 1)
            line_yOffset = line_yOffset->prev;
        else
            cursor->cursY--;
        line_current = line_current->prev;
        if(cursor->cursX > (int)line_current->size - 1)
        {
            cursor->cursX = line_current->size - 1;
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
            line_yOffset = line_yOffset->next;
        else
            cursor->cursY++;
        line_current = line_current->next;
        if(cursor->cursX > (int)line_current->size - 1)
        {
            cursor->cursX = line_current->size - 1;
        }
    }
}

static void move_left(void)
{
    if(cursor->cursX > 0)
        cursor->cursX--;
    else
    {
        if(line_current->prev != line_head)
        {
            line_current = line_current->prev;
            cursor->cursY--;
            cursor->cursX = line_current->size - 1;
        }
    }
}

static void move_right(void)
{
    if(cursor->cursX < (int)line_current->size - 1)
        cursor->cursX++;
    else
    {
        if(line_current->next != NULL)
        {
            line_current = line_current->next;
            cursor->cursY++;
            cursor->cursX = 0;
        }
    }
}

static void tab(void)
{
    memmove(&line_current->buffer[cursor->cursX + TAB_WIDTH], &line_current->buffer[cursor->cursX], line_current->size - cursor->cursX); // Must memmove because of '\n' character
    for(int i = cursor->cursX; i < cursor->cursX + TAB_WIDTH; i++)
        line_current->buffer[i] = ' ';
    cursor->cursX += TAB_WIDTH;
    line_current->size += TAB_WIDTH;
}
