/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "init.h"
#include "line.h"

void init(void)
{
    // Initialize NCURSES
    initscr();
    cbreak();
    noecho();
    
    init_t = (Init *)malloc(sizeof(*init_t));
    memset(init_t, 0, sizeof(*init_t));

    getmaxyx(stdscr, init_t->termY, init_t->termX);
}

void init_args(int argc, const char *argv[])
{
    if(argc > 1)
    {
        if(argv[1][0] == '-')
        {   
            clean();
            printf("HELP");
            exit(1);
        }
        else
        {
            strncpy(init_t->filename, argv[1], MAX_FILENAME - 1);
        }
    }
}

void init_line(void)
{
    line_head = malloc(sizeof(*line_head));
    memset(line_head, 0, sizeof(*line_head));
    line_tail = line_head;
}

void clean(void)
{
    endwin();
}
