/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "init.h"
#include "line.h"
#include "curses.h"

void init(void)
{
    init_t = (Init *)malloc(sizeof(*init_t));
    memset(init_t, 0, sizeof(*init_t));
}

void clean(void)
{
    curses_clean();
}
