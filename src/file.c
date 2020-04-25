#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "input.h"
#include "buffer.h"
#include "line.h"
#include "file.h"


void file_init(void)
{
   file = (file_t *)malloc(sizeof(*file));
   memset(file, 0, sizeof(*file));
}

void file_save(void)
{
    line_t *tmp;
    FILE *f = fopen(file->filename, "w");

    if (f == NULL)
    {
        fprintf(stderr, "Error while saving %s: %s\n", file->filename, strerror(errno));
        sleep(3);
        exit(-1);
        return;
    }

    for(tmp = line_head->next; tmp != NULL; tmp = tmp->next)
    {
        fprintf(f, "%s", tmp->buffer);
    }
    fclose(f);
    //exit(1);
}

