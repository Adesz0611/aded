#include <stdio.h>

#include "line.h"
#include "curses.h"
#include "file.h"
#include "defs.h"

void display_buffer(void)
{
    int i, n;
    line_t *tmp;

    for(n = 0; n < curses->termY; n++)
    {
        for(i = 0; i < curses->termX; i++)
        {
            #if TILDE_IN_BLANK_LINE
            if(i == 0)
                mvprintw(n, i, "~");
            else
            #endif
                mvprintw(n, i, " ");
        }
    }

    for(i = 0, tmp = line_head->next; i < curses->termY && tmp != NULL; i++, tmp = tmp->next)
    {
        for(n = 0; n < curses->termX && tmp->buffer[n] != '\0'; n++)
        {
            #if DISPLAY_DOLLAR_AS_NEWLINE
            if(tmp->buffer[n] == '\n')
                mvprintw(i, n, "$");
            else
            #endif
                mvprintw(i, n, "%c", tmp->buffer[n]);
        }
    }
}
