#pragma once
#define MAXLINE 4096
#define MAX_FILENAME 256 // Changed from 64 bytes

// KEYS
#define ENTER 10
#define KEY_TAB 9

#define TAB_WIDTH 4

// USERS MACRO
#define XSCROLL_VALUE 3 /* Horizontal scroll value */
#define DISPLAY_DOLLAR_AS_NEWLINE 0
#define TILDE_IN_BLANK_LINE 1
#define ALLOW_HOME_AND_END_KEY 1

#define scpy(dst, src) strncpy((dst), (src), (sizeof(dst)) - 1)
