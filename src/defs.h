#ifndef DEFS_H
#define DEFS_H

#include "curses.h" // For macros

#define MAXLINE 4096
#define MAX_FILENAME 256 // Changed from 64 bytes

// KEYS
#define ENTER 10
#define KEY_TAB 9

#define TAB_WIDTH 4

// USERS MACRO
#define XSCROLL_VALUE 5//WINDOW_WIDTH(main_window) / 2 /* Horizontal scroll value */
#define ONEPAGE_VALUE WINDOW_HEIGHT(main_window)  / 2
#define DISPLAY_PILCROW_AS_NEWLINE 0
#define TILDE_IN_BLANK_LINE 0
#define ALLOW_HOME_AND_END_KEY 1

#define scpy(dst, src) strncpy((dst), (src), (sizeof(dst)) - 1)

#endif
