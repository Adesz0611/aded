/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#pragma once
#include <stdbool.h>

#include "defs.h"


typedef struct {
    char filename[MAX_FILENAME];
    size_t name_size;       /* Filename's size in bytes */
    size_t name_length;     /* Filename's length in characters */

    bool fileExist;
} file_t;

file_t *file;

void file_init(void);
bool file_exist(const char *path);
void file_save(void);
void file_load(const char *path);
void file_clean(void);
