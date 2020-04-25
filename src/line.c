#include <stdlib.h>
#include <string.h>

#include "line.h"


void line_init (void)
{
    line_head = malloc(sizeof(*line_head));
    memset(line_head, 0, sizeof(*line_head));
    line_tail = line_head;
}

line_t line_add(const char *text)
{
    
}

