#pragma once

#include <stdio.h>
#include <stdlib.h>

#define FROM_BACKSPACE_KEY 1
#define FROM_DEFAULT 0

typedef struct line_t{
    char *buffer;
    size_t size;
    struct line_t *prev;
    struct line_t *next;
} line_t;

void line_init(void);
line_t *line_add(const char *text);
void line_delete(int flag);

line_t *line_head;
line_t *line_tail;
line_t *line_current;
