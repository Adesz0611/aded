/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#include <stdlib.h>
#include <string.h>

#include "buffer.h"

buffer_t *buffer;

void buffer_init()
{
    buffer = buffer_add();
}

buffer_t *buffer_add()
{
    buffer_t *p_buf = (buffer_t *)malloc(sizeof(*p_buf));
    memset(p_buf, 0, sizeof(*p_buf));
    p_buf->cursX = p_buf->cursY = 0;
    p_buf->numlines = 0;

    return p_buf;
}

void buffer_clean(buffer_t *p_buf)
{
    free(p_buf);
}
