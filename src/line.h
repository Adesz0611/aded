#pragma once

#include <stdio.h>

typedef struct line_t{
    char *buffer;
    size_t size;
    struct line_t *prev;
    struct line_t *next;
} line_t;

void line_init(void);
line_t *line_add(const char *text);

line_t *line_head;
line_t *line_tail;
line_t *line_current;
