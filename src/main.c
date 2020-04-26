/*
 *  (C) 2020 Adam Hunyadvari
 *     <adesz@jss.hu>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "types.h"
#include "defs.h"
#include "line.h"
#include "curses.h"
#include "print.h"
#include "display.h"
#include "buffer.h"
#include "input.h"
#include "file.h"

static void destroy(void);

int main (int argc, const char *argv[])
{
    atexit(destroy);

    // init
    file_init();
    
    if(argc > 1)
    {
        if(!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
        {
            print_usage();
            return EXIT_FAILURE;
        }
        
        else if(!strcmp(argv[1], "--version"))
        {
            print_version();
            return EXIT_FAILURE;
        }
        else
        {
            scpy(file->filename, argv[1]);
        }
    }
    curses_init();
    buffer_init();
    line_init();

    line_add(""); // First line


    while(1)
    {
       display_buffer();
       move(buffer->cursY, buffer->cursX);
       input();
    }
    
    return 0;
}

static void destroy(void)
{
    // Destroy
    curses_clean();
}
