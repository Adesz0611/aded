/*
 *  (C) 2020 Adam Hunyadvari
 *     <adesz@jss.hu>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <locale.h>

#include "types.h"
#include "defs.h"
#include "line.h"
#include "curses.h"
#include "display.h"
#include "buffer.h"
#include "input.h"
#include "file.h"
#include "version.h"
#include "statusbar.h"
#include "cursor.h"
#include "utf8.h"

#if __unix__
#include <signal.h>
static void emergencyexit();
#endif

static void usage(void);
static void destroy(void);

int main (int argc, const char *argv[])
{
    atexit(destroy);
    setlocale(LC_ALL, "");

#if __unix__
    signal(SIGHUP, emergencyexit);
    signal(SIGTERM, emergencyexit);
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
            
            file->name_length = utf8_strlen(file->filename);
            file->name_size = strlen(file->filename);
        }
    }
    curses_init();
    cursor_init();
    buffer_init();
    line_init();
    statusbar_init(STBAR_POS_BOTTOM);

    line_add(""); // First line
    offset->line_yOffset = line_head->next;

    statusbar_display();
    
    while(1)
    {
        display_buffer();
        statusbar_display();
        wmove(main_window->window, cursor->cursY, cursor->cursX);
        input();
    }
    
    return 0;
}

#if __unix__
static void emergencyexit()
{
    // Doesn't ask that you whould like to quit
    // just exit without saving
    exit(EXIT_FAILURE);
}
#endif

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
