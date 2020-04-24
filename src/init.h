/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */

#pragma once
#include "defs.h"

typedef struct {
    char filename[MAX_FILENAME];
    int termX;
    int termY;
} Init;

void init(void);
void init_args(int argc, const char *argv[]);
void init_line(void);
void clean(void);

Init *init_t;
