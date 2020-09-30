/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 *
 *  buffer.c: functions for dealing with buffers
 *
 *
 *  Buffers kept in a doubly linked list. As I mentioned in buffer.h
 */
#include <stdlib.h>
#include <string.h>

#include "buffer.h"

buffer_t *buffer_head;
buffer_t *buffer;
buffer_t *buffer_tail;

/* This function allocates memory for the first node of buffers */
static void buffer_addFirstBuffer(void)
{
    buffer_head = (buffer_t *)malloc(sizeof(*buffer_head));
    memset(buffer_head, 0, sizeof(*buffer_head));
}

/* Initialize the buffers' list */
void buffer_init(void)
{
    buffer_addFirstBuffer();
    buffer_tail = buffer_head;
    buffer = buffer_tail;
}

/* Add a buffer to the linked list */
buffer_t *buffer_add(void)
{
    buffer_t *p_buf = (buffer_t *)malloc(sizeof(*p_buf));
    memset(p_buf, 0, sizeof(*p_buf));

    if(buffer == buffer_tail)
    {
        buffer_tail->next = p_buf;
        p_buf->prev = buffer_tail;
        buffer_tail = p_buf;
    }
    else
    {
        buffer_t *buff_n = buffer->next;

        buffer->next = p_buf;
        p_buf->prev = buffer;
        p_buf->next = buff_n;
        buff_n->prev = p_buf;
    }

    return p_buf;
}

/* This function deletes the given buffer struct from the memory and
 * returns the given buffer's content */
buffer_t buffer_delete(buffer_t *p_buf)
{
    buffer_t ret_buf = *p_buf;

    if(p_buf == buffer_tail)
        buffer_tail->next = NULL;
    else
    {
        p_buf->prev->next = p_buf->next;
        p_buf->next->prev = p_buf->prev;
    }

    free(p_buf);
    return ret_buf;
}

/* Deallocate all of memory that this linked list used */
void buffer_clean(void)
{
    buffer_t *tmp = buffer_head;
    buffer_t *tmp_next;

    while(tmp != NULL)
    {
        tmp_next = tmp->next;
        free(tmp);
        tmp = tmp_next;
    }
}
