/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#include <stdlib.h>
#include <string.h>

#include "statusbar.h"
#include "file.h"
#include "buffer.h"
#include "cursor.h"

Statusbar *statusbar;

void statusbar_init(enum statusbar_position_enum stbar_pos)
{
    statusbar = (Statusbar *)malloc(sizeof(*statusbar));
    memset(statusbar, 0, sizeof(*statusbar));

    statusbar->position = stbar_pos;

    //statusbar->window = curses_windowAdd(1, termInfo->width, termInfo->height - 1, 0);
    statusbar->window = newwin(1, getmaxx(stdscr), getmaxy(stdscr) - 1, 0);
}

void statusbar_display(void)
{
    int i, pos_len;
    char pos[64];

    snprintf(pos, 63, "%ld, %ld ", buffer->cursY + 1, buffer->cursX + 1);
    pos_len = strlen(pos);

    wattron(statusbar->window, A_REVERSE);
    mvwprintw(statusbar->window, 0, 0, " %s", file->filename);

    for(i = file->name_length + 1; i < STATUSBAR_WINDOW_WIDTH - pos_len; i++)
        mvwprintw(statusbar->window, 0, i, " ");
    mvwprintw(statusbar->window, 0, i, "%s", pos);

    wattroff(statusbar->window, A_REVERSE);

    wrefresh(statusbar->window);
}

void statusbar_clean(void)
{
    //free(statusbar->window);
    delwin(statusbar->window);
    free(statusbar);
}
