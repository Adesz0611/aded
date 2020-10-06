/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#include <stdlib.h>
#include <string.h>

#include "statusbar.h"
//#include "file.h"
#include "buffer.h"
#include "cursor.h"

#define ROWS_COLS_MARGIN 15

Statusbar *statusbar;

/* Initialize the statusbar */
/* Allocate memory for the statusbar struct and create window for it */
void statusbar_init(enum statusbar_position_enum stbar_pos)
{
    statusbar = (Statusbar *)malloc(sizeof(*statusbar));
    memset(statusbar, 0, sizeof(*statusbar));

    statusbar->position = stbar_pos;

    statusbar->window = newwin(1, getmaxx(stdscr), getmaxy(stdscr) - 1, 0);
}

/* Displays the current row and current column */
void statusbar_rows_cols(void)
{
    wattron(statusbar->window, A_REVERSE);
    wmove(statusbar->window, 0, STATUSBAR_WINDOW_WIDTH - ROWS_COLS_MARGIN);
    wprintw(statusbar->window, "%d, %d", buffer->cursY + 1, buffer->cursX + 1);
    int tmp_x = getcurx(statusbar->window);
    for(int i = 0; i < STATUSBAR_WINDOW_WIDTH - tmp_x; i++)
        waddch(statusbar->window, ' ');

    wattroff(statusbar->window, A_REVERSE);
    wrefresh(statusbar->window);
}

/* Display the hole statusbar */
void statusbar_display(void)
{
    int i;
    
    wattron(statusbar->window, A_REVERSE);
    mvwprintw(statusbar->window, 0, 0, " %s", buffer->filename);

    for(i = buffer->name_length + 1; i < STATUSBAR_WINDOW_WIDTH - ROWS_COLS_MARGIN; i++)
        waddch(statusbar->window, ' ');

    statusbar_rows_cols();

    wrefresh(statusbar->window);
}

/* Deletes the statusbar's window and deallocate/free the memory */
void statusbar_clean(void)
{
    delwin(statusbar->window);
    free(statusbar);
}
