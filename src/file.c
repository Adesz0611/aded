/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef __unix__
#include <unistd.h>
#endif

#include "input.h"
#include "buffer.h"
#include "line.h"
#include "file.h"


void file_init(void)
{
   file = (file_t *)malloc(sizeof(*file));
   memset(file, 0, sizeof(*file));
}

bool file_exist(const char *path)
{
    FILE *tmp_file = fopen(path, "r");
    if(tmp_file == NULL)
        return false;
    fclose(tmp_file);
    return true;
}

void file_save(void)
{
    line_t *tmp;
    FILE *f = fopen(file->filename, "w");

    if (f == NULL)
    {
        fprintf(stderr, "Error while saving %s: %s\n", file->filename, strerror(errno));
        sleep(3);
        exit(EXIT_FAILURE);
        return;
    }

    for(tmp = line_head->next; tmp != NULL; tmp = tmp->next)
    {
        fprintf(f, "%s", tmp->buffer);
    }
    fclose(f);
}

void file_load(const char *path)
{
    FILE *tmp_file = fopen(path, "r");
    line_t *tmp_line = line_head->next;
    wchar_t c;
    size_t i = 0;

    // Check the file is loaded
    if(tmp_file == NULL)
    {
        fprintf(stderr, "Error while loading %s: %s\n", file->filename, strerror(errno));
        sleep(3);
        exit(EXIT_FAILURE);
    }

    // Load the file to the buffer
    while((c = fgetc(tmp_file)) != EOF)
    {
        tmp_line->buffer[i] = c;
        tmp_line->size++;

        i++;
        if(c == '\n')
        {
            tmp_line = line_addFile();
            i = 0;
        }
    }

    line_tail = tmp_line->prev;
    line_tail->next = NULL;
    free(tmp_line);

    fclose(tmp_file);
}

void file_clean(void)
{
    free(file);
}
