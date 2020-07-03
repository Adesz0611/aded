/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#pragma once

#include "curses.h"
#include "line.h"

enum scroll_direction {
    FORWARD = 0,
    BACKWARD
};

void display_buffer(ADED_WINDOW *win);

//-- Blank --//
void display_blankRow(ADED_WINDOW *win, int y, int x, int n);

//-- Draw --//
void display_line(ADED_WINDOW *win, line_t *line, int row);

void display_scroll(ADED_WINDOW *win, enum scroll_direction direction);
