/*
 *  (C) 2020 Adam Hunyadvari
 *     <adesz@jss.hu>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ncurses.h>

#include "init.h"
#include "types.h"
#include "defs.h"

int main (int argc, const char *argv[]) {
    init();
    init_args(argc, argv);
    getch();
    clean();
    return 0;
}
