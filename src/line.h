#ifndef LINE_H
#define LINE_H

#include <stdio.h>
#include <stdlib.h>

#include "buffer.h"

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

typedef struct {
    line_t *line_yOffset;
    unsigned xOffset;
} Offset;

extern line_t *line_head;
extern line_t *line_tail;
extern line_t *line_current;

extern Offset *offset;


void line_init(void);
line_t *line_add(const char *text, buffer_t *p_buffer);
line_t *line_addFile(buffer_t *p_buffer);
void line_delete(enum line_delete_flag flag, buffer_t *p_buffer);
void line_clean(void);

#endif
