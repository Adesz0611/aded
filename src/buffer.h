/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>

#include "line_defs.h"
#include "defs.h"

/* Buffer contains the file's text. It is a doubly linked list structure.
 * Also contains the cursor position in the file. And the number of lines
 * in the buffer.
 */
typedef struct buffer_t {
    struct buffer_t *next; /* Link to the next buffer struct */
    struct buffer_t *prev; /* Link to the previous buffer struct */

    // Cursor positions in the file
    size_t cursX; /* Current column in the file */
    size_t cursY; /* Current row in the file */

    size_t cursXsh; /* Column where cursor should be */

    // File
    char filename[MAX_FILENAME]; /* File's name that is open in the buffer */
    size_t name_size; /* Filename's size in bytes */
    size_t name_length; /* Filename's length in characters */

    // Line pointers to linked list's element
    line_t *line_head; /* The first/head element of line's doubly linked list */
    line_t *line_tail; /* The last/tail element of line's doubly linked list */
    line_t *line_current; /* The pointer of current line where cursor is */

    // Offsets
    line_t *line_yOffset; /* The top line of window (Vertical offset) */
    size_t xOffset; /* Horizontal offset */

    bool fileExist;

    size_t numlines; // Number of lines in the buffer
} buffer_t;

extern buffer_t *buffer_head;
extern buffer_t *buffer;
extern buffer_t *buffer_tail;

void buffer_init();
buffer_t *buffer_add();
void buffer_clean();
#endif
