/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#include <stdlib.h>
#include <string.h>

#include "statusbar.h"

void statusbar_init(enum statusbar_position_enum stbar_pos)
{
    statusbar = (Statusbar *)malloc(sizeof(*statusbar));
    memset(statusbar, 0, sizeof(*statusbar));

    statusbar->position = stbar_pos;

    statusbar->window = curses_windowAdd(1, termInfo->width, termInfo->height - 1, 0);
}

void statusbar_display(void)
{
    int i;

    wattron(statusbar->window->window, A_REVERSE);
    for(i = 0; i < statusbar->window->width; i++)
        mvwprintw(statusbar->window->window, 0, i, " ");
    wattroff(statusbar->window->window, A_REVERSE);
    
    wrefresh(statusbar->window->window);
}
