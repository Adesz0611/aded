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
    mvprintw(0, 0, "u8: %ld", sizeof(u8));
    mvprintw(1, 0, "u16: %ld", sizeof(u16));
    mvprintw(2, 0, "u32: %ld", sizeof(u32));
    mvprintw(3, 0, "u64: %ld", sizeof(u64));
    getch();
    clean();
    return 0;
}
