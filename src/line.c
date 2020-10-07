/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#include <stdlib.h>
#include <string.h>

#include "line.h"
#include "line_defs.h"
#include "defs.h"
#include "buffer.h"
#include "cursor.h"
#include "curses.h"


void line_init (buffer_t *p_buff)
{
    p_buff->line_head = (line_t *)malloc(sizeof(*p_buff->line_head));
    memset(p_buff->line_head, 0, sizeof(*p_buff->line_head));

    p_buff->xOffset = 0;

    p_buff->line_tail = p_buff->line_head;
    p_buff->line_current = p_buff->line_head;
}

line_t *line_add(const char *text, buffer_t *p_buff)
{
    line_t *tmp = (line_t *)malloc(sizeof(*tmp));
    memset(tmp, 0, sizeof(*tmp));

    tmp->buffer = malloc(MAXLINE);
    memset(tmp->buffer, 0, MAXLINE);

    strncpy(tmp->buffer, text, MAXLINE - 1);
    tmp->size = strlen(text) + 1;

    tmp->buffer[tmp->size - 1] = '\n';


    if(p_buff->line_current == p_buff->line_tail)
    {
        p_buff->line_tail->next = tmp;
        tmp->prev = p_buff->line_tail;
        p_buff->line_tail = tmp;
    }

    // If it isn't the line_tail
    else
    {
        line_t *tmp_next = p_buff->line_current->next;
        p_buff->line_current->next = tmp;
        tmp->prev = p_buff->line_current;
        tmp->next = tmp_next;
        tmp_next->prev = tmp;
    }
     
    p_buff->line_current = tmp;
    p_buff->numlines++;

    return tmp;
}

line_t *line_addFile(buffer_t *p_buff)
{
    line_t *tmp = (line_t *)malloc(sizeof(*tmp));
    memset(tmp, 0, sizeof(*tmp));

    tmp->buffer = malloc(MAXLINE);
    memset(tmp->buffer, 0, MAXLINE);

    tmp->size = 0;

    p_buff->line_tail->next = tmp;
    tmp->prev = p_buff->line_tail;
    p_buff->line_tail = tmp;
    
    p_buff->numlines++;

    return tmp;
}

/* Line delete stupid solution*/
// TODO: line_t return value (?)
void line_delete(enum line_delete_flag flag, buffer_t *p_buff)
{
    // Pointer to the line that will be deleted
    line_t *tmp;

    if(flag != BY_DELETE)
    {
        p_buff->line_current = p_buff->line_current->prev;
        tmp = p_buff->line_current->next;

        /* ------------------------------ */

        if(WINDOW_WIDTH(main_window) < (int)p_buff->line_current->size - 1)
        {
            cursor->cursX = WINDOW_WIDTH(main_window) - XSCROLL_VALUE;
            p_buff->xOffset = p_buff->line_current->size - XSCROLL_VALUE - 1;
        }
        else
        {
            cursor->cursX = p_buff->line_current->size - 1;
        }
        
        buffer->cursX = p_buff->line_current->size - 1;
    }
    else
    {
        if(p_buff->line_current != p_buff->line_tail)
            tmp = p_buff->line_current->next;
        else
            return;
    }
   
    if(flag == BY_BACKSPACE || flag == BY_DELETE)
    {
        // Move the line's buffer that be deleted to the prev line's end
        // We don't use memmove() here because the memory don't overlap
        memcpy(&p_buff->line_current->buffer[buffer->cursX], &tmp->buffer[0], tmp->size - 1); // -1 Because of '\n' character
        p_buff->line_current->size += tmp->size - 1; // +1 for '\n'
        p_buff->line_current->buffer[p_buff->line_current->size - 1] = '\n';
    }

    if(tmp->next != NULL)
    {
        p_buff->line_current->next = tmp->next;
        // Set current->next's prev :)
        p_buff->line_current->next->prev = p_buff->line_current;
    }
    else
    {
        p_buff->line_current->next = NULL;
        p_buff->line_tail = p_buff->line_current;
    }

    free(tmp->buffer);
    free(tmp);
    p_buff->numlines--;

    if(flag != BY_DELETE)
    {   
        if(cursor->cursY < 1)
            p_buff->line_yOffset = p_buff->line_yOffset->prev;
        else
           cursor->cursY--;
        buffer->cursY--;
    }
}


/* This function free all of used line memory
   ONLY use at exit!                          */
void line_clean(buffer_t *p_buff)
{
    line_t *tmp = p_buff->line_head->next;
    line_t *next_tmp = NULL;
    
    // First free the line_head which hasn't got buffer
    free(p_buff->line_head);

    // First free the lines
    for( ; tmp != NULL; )
    {
        next_tmp = tmp->next;
        free(tmp->buffer);
        free(tmp);
        tmp = next_tmp;
    }
}
