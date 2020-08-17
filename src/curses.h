/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#ifndef CURSES_H
#define CURSES_H

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

extern TerminalInfo *termInfo;
extern ADED_WINDOW *main_window;

void curses_init(void);

// Window stuffs
ADED_WINDOW *curses_windowAdd(int height, int width, int startY, int startX);
void curses_windowDelete(ADED_WINDOW *win);
void curses_windowResize(ADED_WINDOW *win, int height, int width);

void curses_resize();
void curses_clean(void);

#endif
