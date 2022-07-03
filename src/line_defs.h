#ifndef LINE_DEFS_H
#define LINE_DEFS_H

#include <stdio.h>

enum line_delete_flag {
    BY_DEFAULT = 0,
    BY_BACKSPACE,
    BY_DELETE
};

typedef struct line_t{
    char *buffer;
    size_t size; /* Number of bytes in buffer */
    size_t sizeCh; /* Number of characters in buffer */
    struct line_t *prev;
    struct line_t *next;
} line_t;

#endif
