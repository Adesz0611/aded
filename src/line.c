#include <stdlib.h>
#include <string.h>

#include "line.h"
#include "defs.h"


void line_init (void)
{
    line_head = malloc(sizeof(*line_head));
    memset(line_head, 0, sizeof(*line_head));
    line_tail = line_head;
}

line_t *line_add(const char *text)
{
    line_t *tmp = (line_t *)malloc(sizeof(*tmp));
    memset(tmp, 0, sizeof(*tmp));

    tmp->buffer = malloc(MAXLINE);
    memset(tmp->buffer, 0, MAXLINE);

    strncpy(tmp->buffer, text, MAXLINE - 1);
    tmp->size = strlen(text);

    line_tail->next = tmp;
    tmp->prev = line_tail;
    line_tail = tmp;
    line_current = tmp;

    return tmp;
}

