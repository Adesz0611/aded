#include <stdio.h>

#include "line.h"
#include "curses.h"
#include "file.h"
#include "defs.h"

void display_buffer(void)
{
    int i, n;
    line_t *tmp;

    for(n = 0; n < main_window->height; n++)
    {
        for(i = 0; i < main_window->width; i++)
        {
            #if TILDE_IN_BLANK_LINE
            if(i == 0)
                mvwprintw(main_window->window, n, i, "~");
            else
            #endif
                mvwprintw(main_window->window, n, i, " ");
        }
    }

    for(i = 0, tmp = offset->line_yOffset; i < main_window->height && tmp != NULL; i++, tmp = tmp->next)
    {
        for(n = 0; n < main_window->width && tmp->buffer[n] != '\0'; n++)
        {
            #if DISPLAY_DOLLAR_AS_NEWLINE
            if(tmp->buffer[n] == '\n')
                mvwprintw(main_window->window, i, n, /*"¶"*/"$");
            else
            #endif
                mvwprintw(main_window->window, i, n, "%c", tmp->buffer[n]);
        }
    }
}
