/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#ifndef DISPLAY_H
#define DISPLAY_H

#include "curses.h"
#include "buffer.h"
#include "line.h"

#define full_redraw(win)        display_buffer(win, buffer->line_yOffset, getbegy(win), WINDOW_HEIGHT(win));

#define display_page_up(win)    display_onepage(win, BACKWARD)
#define display_page_down(win)  display_onepage(win, FORWARD)

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

void display_onepage(WINDOW *win, enum scroll_direction direction);

void display_position_cursor_horizontally(WINDOW *win, buffer_t *p_buffer, bool shouldRedraw);

#endif
