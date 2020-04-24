#pragma once
#include <stdio.h>
typedef struct {
    char *buffer;
    size_t size;
    struct line_t *prev;
    struct line_t *next;
} line_t;

line_t *line_head = NULL;
line_t *line_tail = NULL;
line_t *line_current = NULL;

line_t line_add(const char *text);
