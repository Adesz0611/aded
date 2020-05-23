#pragma once

#include <ncurses.h>

typedef struct {
    WINDOW *window;
    int termX;
    int termY;
} curses_t;

curses_t *curses;

void curses_init(void);
void curses_resize();
void curses_clean(void);
