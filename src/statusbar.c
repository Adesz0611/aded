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

void statusbar_init(enum statusbar_position_enum stbar_pos)
{
    statusbar = (Statusbar *)malloc(sizeof(*statusbar));
    memset(statusbar, 0, sizeof(*statusbar));

    statusbar->position = stbar_pos;

    statusbar->window = curses_windowAdd(1, termInfo->width, termInfo->height - 1, 0);
}

void statusbar_display(void)
{
    int i, pos_len;
    char pos[64];

    snprintf(pos, 63, "%d, %d ", buffer->cursY + 1, buffer->cursX + 1);
    pos_len = strlen(pos);

    wattron(statusbar->window->window, A_REVERSE);
    mvwprintw(statusbar->window->window, 0, 0, " %s", file->filename);

    for(i = file->name_length + 1; i < statusbar->window->width - pos_len; i++)
        mvwprintw(statusbar->window->window, 0, i, " ");
    mvwprintw(statusbar->window->window, 0, i, "%s", pos);

    wattroff(statusbar->window->window, A_REVERSE);

    wrefresh(statusbar->window->window);
}

void statusbar_clean(void)
{
    free(statusbar->window);
    free(statusbar);
}
