#include <stdlib.h>
#include <string.h>

#include "buffer.h"

void buffer_init(void)
{
   buffer = (buffer_t *)malloc(sizeof(*buffer));
   memset(buffer, 0, sizeof(*buffer));
   buffer->cursX = buffer->cursY = 0;
}