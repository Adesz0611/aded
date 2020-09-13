/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#ifndef CURSES_H
#define CURSES_H

#include <ncurses.h>

// Macros for stdscr's size
#ifdef STDSCR_WIDTH
#undef STDSCR_WIDTH
#endif

#ifdef STDSCR_HEIGHT
#undef STDSCR_HEIGHT
#endif

#define STDSCR_WIDTH    getmaxx(stdscr)
#define STDSCR_HEIGHT   getmaxy(stdscr)
//-------------------------

// Macros for window's size
#ifdef WINDOW_WIDTH
#undef WINDOW_WIDTH
#endif

#ifdef WINDOW_HEIGHT
#undef WINDOW_HEIGHT
#endif

#define WINDOW_WIDTH(win)   getmaxx(win)
#define WINDOW_HEIGHT(win)  getmaxy(win)
//-------------------------

extern WINDOW *main_window;

void curses_init(void);

void curses_resize();
void curses_clean(void);

#endif
