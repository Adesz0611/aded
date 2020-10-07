/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#ifndef LINE_H
#define LINE_H

#include <stdio.h>
#include <stdlib.h>

#include "buffer.h"


void line_init(buffer_t *p_buff);
line_t *line_add(const char *text, buffer_t *p_buff);
line_t *line_addFile(buffer_t *p_buff);
void line_delete(enum line_delete_flag flag, buffer_t *p_buff);
void line_clean(buffer_t *p_buff);

#endif
