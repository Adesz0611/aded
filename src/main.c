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
#include "debug.h"

#if __unix__
#include <signal.h>
static void emergencyexit();
#endif

static void usage(void);
static void destroy(void);

int main (int argc, const char *argv[])
{
    setlocale(LC_ALL, "");

#if __unix__
    signal(SIGHUP, emergencyexit);
    signal(SIGTERM, emergencyexit);
#endif

    
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
            // init
            file_init();
            line_init();
            buffer_init();


            scpy(file->filename, argv[1]);
 
            file->name_length = utf8_strlen(file->filename);
            file->name_size = strlen(file->filename);

            // Check the file is exist or not
            if(file_exist(file->filename))
            {
                offset->line_yOffset = line_current = line_addFile(buffer); // First line
                file->fileExist = true;
                file_load(file->filename);
            }
            else
            {
                offset->line_yOffset = line_add("", buffer); // First line
                file->fileExist = false;
            }
        }
    }
    
    // If there is no argument
    else
    {
        // init
        file_init();
        line_init();
        buffer_init();

        offset->line_yOffset = line_current = line_add("", buffer); // First line
    }


    atexit(destroy);


    curses_init();
    cursor_init();
    statusbar_init(STBAR_POS_BOTTOM);

    
    display_buffer(main_window, offset->line_yOffset, 0, WINDOW_HEIGHT(main_window));
    
    while(1)
    {
        statusbar_display();
        wmove(main_window, cursor->cursY, cursor->cursX);
        DEBUGF("%d\n", buffer->numlines);
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
    // Clean up everything
    curses_clean();
    line_clean();
    buffer_clean();
    cursor_clean();
    file_clean();
    statusbar_clean();
}
