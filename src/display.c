#include <stdio.h>

#include "line.h"
#include "curses.h"
#include "file.h"

void display_buffer(void)
{
    int i, n;
    line_t *tmp;

    for(n = 0; n < curses->termY; n++)
    {
        for(i = 0; i < curses->termX; i++)
        {
            mvprintw(n, i, " ");
        }
    }

    for(i = 0, tmp = line_head->next; i < curses->termY && tmp != NULL; i++, tmp = tmp->next)
    {
        for(n = 0; n < curses->termX && tmp->buffer[n] != '\0'; n++)
        {
            mvprintw(i, n, "%c", tmp->buffer[n]);
        }
    }
}
