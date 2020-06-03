/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#include <stdlib.h>
#include <string.h>

#include "cursor.h"

void cursor_init(void)
{
    cursor = (Cursor *)malloc(sizeof(*cursor));
    memset(cursor, 0, sizeof(*cursor));

    cursor->cursX = cursor->cursY = 0;
}
