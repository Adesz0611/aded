/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#include <stdlib.h>
#include <string.h>

#include "buffer.h"

buffer_t *buffer;

void buffer_init(void)
{
   buffer = (buffer_t *)malloc(sizeof(*buffer));
   memset(buffer, 0, sizeof(*buffer));
   buffer->cursX = buffer->cursY = 0;
}

void buffer_clean(void)
{
    free(buffer);
}
