#pragma once

#include "defs.h"


typedef struct {
    char filename[MAX_FILENAME]; 
} file_t;

file_t *file;

void file_init(void);
void file_save(void);
