/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#ifndef BUFFER_H
#define BUFFER_H

typedef struct {
    int cursX;
    int cursY;
} buffer_t;

extern buffer_t *buffer;

void buffer_init(void);
void buffer_clean(void);
#endif
