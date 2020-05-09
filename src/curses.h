#pragma once

#include <ncurses.h>
#include <signal.h>

typedef struct {
    WINDOW *window;
    // window!
    int termX;
    int termY;
} curses_t;

curses_t *curses;

void curses_init(void);
#if __unix__
#ifdef SIGWINCH
void curses_resize();
#endif
#endif
void curses_clean(void);
