#pragma once

#include <stdio.h>
#include <stdlib.h>

enum line_delete_flag {
    BY_DEFAULT = 0,
    BY_BACKSPACE,
    BY_DELETE
};

typedef struct line_t{
    char *buffer;
    size_t size;
    struct line_t *prev;
    struct line_t *next;
} line_t;

void line_init(void);
line_t *line_add(const char *text);
void line_delete(enum line_delete_flag flag);

line_t *line_head;
line_t *line_tail;
line_t *line_current;
line_t *line_yOffset;
