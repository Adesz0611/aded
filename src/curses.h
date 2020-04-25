#pragma once

#include <ncurses.h>

typedef struct {
    WINDOW *window;
    // window!
    int termX;
    int termY;
} curses_t;

curses_t *curses;

void curses_init(void);
void curses_one_loop(void);
void curses_clean(void);
