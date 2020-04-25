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

void clean(void);

Init *init_t;
