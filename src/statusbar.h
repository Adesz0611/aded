/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 *
 *  This file contains all of routines
 *  that connects to status bar.
 *       
 */
#ifndef STATUSBAR_H
#define STATUSBAR_H

#include "curses.h"

// Macros for statusbar's size
#define STATUSBAR_WINDOW_WIDTH  getmaxx(statusbar->window)
#define STATUSBAR_WINDOW_HEIGHT getmaxy(statusbar->window)

enum statusbar_position_enum {
    STBAR_POS_BOTTOM = 0,
    STBAR_POS_TOP
};

typedef struct _Statusbar {
    WINDOW *window;
    enum statusbar_position_enum position;
} Statusbar;

extern Statusbar *statusbar;

void statusbar_init(enum statusbar_position_enum stbar_pos);
void statusbar_rows_cols(void);
void statusbar_display(void);
void statusbar_clean(void);

#endif
