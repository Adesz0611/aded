/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#ifndef FILE_H
#define FILE_H

#include <stdbool.h>

#include "buffer.h"
#include "defs.h"


bool file_exist(const char *path);
void file_save(buffer_t *p_buff);
void file_load(buffer_t *p_buff, const char *path);

#endif
