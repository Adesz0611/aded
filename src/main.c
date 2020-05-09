/*
 *  (C) 2020 Adam Hunyadvari
 *     <adesz@jss.hu>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#if __unix__
#include <signal.h>
#endif

#include "types.h"
#include "defs.h"
#include "line.h"
#include "curses.h"
#include "display.h"
#include "buffer.h"
#include "input.h"
#include "file.h"
#include "version.h"

static void usage(void);
static void destroy(void);

int main (int argc, const char *argv[])
{
    atexit(destroy);

#if __unix__
#ifdef SIGWINCH
    signal(SIGWINCH, curses_resize);
#endif
#endif

    // init
    file_init();
    
    if(argc > 1)
    {
        if(!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
        {
            usage();
            return EXIT_FAILURE;
        }
        
        else if(!strcmp(argv[1], "--version"))
        {
            version();
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
    line_yOffset = line_head->next;

    while(1)
    {
       display_buffer();
       wmove(curses->window, buffer->cursY, buffer->cursX);
       input();
    }
    
    return 0;
}

static void usage(void)
{
    printf("Usage: %s [options] filename\n", PROGRAM_NAME);
    fputs(
        "Options:\n"
        "         --help     display help then exit\n"
        "         --version  display version informations then exit\n",
    stdout);
}

static void destroy(void)
{
    // Destroy
    curses_clean();
}
