/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#include <stdlib.h>
#include <string.h>

#include "statusbar.h"

void statusbar_init(enum statusbar_position_enum stbar_pos)
{
    statusbar = (Statusbar *)malloc(sizeof(*statusbar));
    memset(statusbar, 0, sizeof(*statusbar));

    statusbar->position = stbar_pos;
}
