#pragma once
typedef struct {
    int cursX;
    int cursY;
} buffer_t;

buffer_t *buffer;

void buffer_init(void);
