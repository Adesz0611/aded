/*
 *  (C) 2020 Adam Hunyadvari
 *     <adesz@jss.hu>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "init.h"
#include "types.h"
#include "defs.h"
#include "line.h"
#include "curses.h"
#include "print.h"

int main (int argc, const char *argv[]) {

    if(argc > 1)
    {
        if(!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
        {
            print_usage(EXIT_FAILURE);
        }
        
        else if(!strcmp(argv[1], "--version"))
        {
            print_version(EXIT_FAILURE);
        }
    }
    /*else
    {
        strncpy(init_t->filename, argv[1], MAX_FILENAME - 1);
    }*/

    curses_init();
    line_init();

    while(1)
    {
        curses_one_loop();
    }

    return 0;
}
