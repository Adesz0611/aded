/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#pragma once

#include <ncurses.h>

typedef struct {
    int height;
    int width;
} TerminalInfo;

typedef struct {
    WINDOW *window;
    int height;
    int width;
} ADED_WINDOW;

TerminalInfo *termInfo;
ADED_WINDOW *main_window;

void curses_init(void);
ADED_WINDOW *curses_windowAdd(int height, int width, int startY, int startX);
void curses_resize();
void curses_clean(void);
