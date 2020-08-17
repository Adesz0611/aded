/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 *
 *  This file contains a struct
 *  which contains the "real" cursor positions.
 */
#ifndef CURSOR_H
#define CURSOR_H

typedef struct {
    int cursX;
    int cursY;
} Cursor;

extern Cursor *cursor;

void cursor_init(void);
void cursor_clean(void);

#endif
