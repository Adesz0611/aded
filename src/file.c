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


bool file_exist(const char *path)
{
    FILE *tmp_file = fopen(path, "r");
    if(tmp_file == NULL)
        return false;
    fclose(tmp_file);
    return true;
}

void file_save(buffer_t *p_buff)
{
    line_t *tmp;
    FILE *f = fopen(p_buff->filename, "w");

    if (f == NULL)
    {
        fprintf(stderr, "Error while saving %s: %s\n", p_buff->filename, strerror(errno));
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

void file_load(buffer_t *p_buff, const char *path)
{
    FILE *tmp_file = fopen(path, "r");
    line_t *tmp_line = line_head->next;
    wchar_t c;
    size_t i = 0;

    // Check the file is loaded
    if(tmp_file == NULL)
    {
        fprintf(stderr, "Error while loading %s: %s\n", p_buff->filename, strerror(errno));
        sleep(3);
        exit(EXIT_FAILURE);
    }

    fseek(tmp_file, 0, SEEK_END);
    long tmp_size = ftell(tmp_file);
    fseek(tmp_file, 0, SEEK_SET);

    size_t cur_s = 0;

    // Load the file to the buffer
    while((c = fgetc(tmp_file)) != EOF)
    {
        tmp_line->buffer[i] = c;
        tmp_line->size++;

        i++;
        cur_s++;
        if(c == '\n' && (long int)cur_s != tmp_size)
        {
            tmp_line = line_addFile(buffer);
            i = 0;
        }
    }

    if(tmp_line->buffer[tmp_line->size - 1] != '\n')
    {
        tmp_line->buffer[tmp_line->size] = '\n';
        tmp_line->size++;
    }

    fclose(tmp_file);
}
