/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#ifndef BUFFER_H
#define BUFFER_H

typedef struct {
    int cursX;
    int cursY;
    unsigned numlines; // Number of lines in the buffer
} buffer_t;

extern buffer_t *buffer;

void buffer_init();
buffer_t *buffer_add();
void buffer_clean(buffer_t *p_buf);
#endif
