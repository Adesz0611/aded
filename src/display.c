#include <stdio.h>

#include "display.h"
#include "init.h"
#include "line.h"
#include "curses.h"

void display_buffer(void)
{
    int i, n;
    line_t *tmp;

    for(n = 0; n < init_t->termY; n++)
    {
        for(i = 0; i < init_t->termX; i++)
        {
            mvprintw(n, i, " ");
        }
    }

    for(i = 0, tmp = line_head->next; i < init_t->termY && tmp != NULL; i++, tmp = tmp->next)
    {
        for(n = 0; n < init_t->termX && tmp->buffer[n] != '\0'; n++)
        {
            mvprintw(i, n, "%c", tmp->buffer[n]);
        }
    }
}
