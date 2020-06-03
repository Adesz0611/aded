/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 *
 *  This file contains a struct
 *  which contains the "real" cursor positions.
 */
#pragma once

typedef struct {
    int cursX;
    int cursY;
} Cursor;

Cursor *cursor;

void cursor_init(void);
