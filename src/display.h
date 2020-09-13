/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#ifndef DISPLAY_H
#define DISPLAY_H

#include "curses.h"
#include "line.h"

enum scroll_direction {
    FORWARD = 0,
    BACKWARD
};

void display_buffer(WINDOW *win, line_t *line, int y, int nrow);

//-- Blank --//
void display_blankRow(WINDOW *win, int y, int x, int n);

//-- Draw --//
void display_line(WINDOW *win, line_t *line, int y, int x, int n);

void display_scroll(WINDOW *win, enum scroll_direction direction);

#endif
