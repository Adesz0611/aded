/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 *
 *  This file contains all of routines
 *  that connects to status bar.
 *  
 *  Read before start to hack:
 *  
 *  - Use 4 spaces instead of tab
 *  - Only use libncurses, one framework is enough
 *       
 */
#pragma once

enum statusbar_position_enum {
    STBAR_POS_BOTTOM = 0,
    STBAR_POS_TOP
};

typedef struct _Statusbar {
    // Position
    enum statusbar_position_enum position;
} Statusbar;

Statusbar *statusbar;

void statusbar_init(enum statusbar_position_enum stbar_pos);
