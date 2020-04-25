#pragma once

#include "defs.h"

void file_save(void);

typedef struct {
    char filename[MAX_FILENAME]; 
} file_t;

file_t *file;
void file_init(void);

