/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#pragma once
typedef struct {
    int cursX;
    int cursY;
} buffer_t;

buffer_t *buffer;

void buffer_init(void);
