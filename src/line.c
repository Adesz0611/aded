#include <stdlib.h>
#include <string.h>

#include "line.h"
#include "defs.h"
#include "buffer.h"
#include "cursor.h"
#include "curses.h"

line_t *line_head;
line_t *line_tail;
line_t *line_current;

Offset *offset;

void line_init (void)
{
    line_head = (line_t *)malloc(sizeof(*line_head));
    memset(line_head, 0, sizeof(*line_head));

    offset = (Offset *)malloc(sizeof(*offset));
    memset(offset, 0, sizeof(*offset));

    offset->xOffset = 0;

    line_tail = line_head;
    line_current = line_head;
}

line_t *line_add(const char *text)
{
    line_t *tmp = (line_t *)malloc(sizeof(*tmp));
    memset(tmp, 0, sizeof(*tmp));

    tmp->buffer = malloc(MAXLINE);
    memset(tmp->buffer, 0, MAXLINE);

    strncpy(tmp->buffer, text, MAXLINE - 1);
    tmp->size = strlen(text) + 1;

    tmp->buffer[0] = '\n';


    if(line_current == line_tail)
    {
        line_tail->next = tmp;
        tmp->prev = line_tail;
        line_tail = tmp;
    }

    // If it isn't the line_tail
    else
    {
        line_t *tmp_next = line_current->next;
        line_current->next = tmp;
        tmp->prev = line_current;
        tmp->next = tmp_next;
        tmp_next->prev = tmp;
    }
     
    line_current = tmp;

    return tmp;
}

line_t *line_addFile(void)
{
    line_t *tmp = (line_t *)malloc(sizeof(*tmp));
    memset(tmp, 0, sizeof(*tmp));

    tmp->buffer = malloc(MAXLINE);
    memset(tmp->buffer, 0, MAXLINE);

    tmp->size = 0;

    line_tail->next = tmp;
    tmp->prev = line_tail;
    line_tail = tmp;

    return tmp;
}

/* Line delete stupid solution*/

void line_delete(enum line_delete_flag flag)
{
    // Pointer to the line that will be deleted
    line_t *tmp;

    if(flag != BY_DELETE)
    {
        line_current = line_current->prev;
        tmp = line_current->next;

        /* ------------------------------ */

        if(WINDOW_WIDTH(main_window) < (int)line_current->size - 1)
        {
            cursor->cursX = WINDOW_WIDTH(main_window) - XSCROLL_VALUE;
            offset->xOffset = line_current->size - XSCROLL_VALUE - 1;
        }
        else
        {
            cursor->cursX = line_current->size - 1;
        }
        
        buffer->cursX = line_current->size - 1;
    }
    else
    {
        if(line_current != line_tail)
            tmp = line_current->next;
        else
            return;
    }
   
    if(flag == BY_BACKSPACE || flag == BY_DELETE)
    {
        // Move the line's buffer that be deleted to the prev line's end
        // We don't use memmove() here because the memory don't overlap
        memcpy(&line_current->buffer[buffer->cursX], &tmp->buffer[0], tmp->size - 1); // -1 Because of '\n' character
        line_current->size += tmp->size - 1; // +1 for '\n'
        line_current->buffer[line_current->size - 1] = '\n';
    }

    if(tmp->next != NULL)
    {
        line_current->next = tmp->next;
        // Set current->next's prev :)
        line_current->next->prev = line_current;
    }
    else
    {
        line_current->next = NULL;
        line_tail = line_current;
    }

    free(tmp->buffer);
    free(tmp);

    if(flag != BY_DELETE)
    {   
        if(cursor->cursY < 1)
            offset->line_yOffset = offset->line_yOffset->prev;
        else
           cursor->cursY--;
        buffer->cursY--;
    }
}


/* This function free all of used line memory
   ONLY use at exit!                          */
void line_clean(void)
{
    line_t *tmp = line_head->next;
    line_t *next_tmp = NULL;
    
    // First free the line_head which hasn't got buffer
    free(line_head);

    // First free the lines
    for( ; tmp != NULL; )
    {
        next_tmp = tmp->next;
        free(tmp->buffer);
        free(tmp);
        tmp = next_tmp;
    }

    // Free the offset struct
    free(offset);
}
