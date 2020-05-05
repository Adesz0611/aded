#pragma once
#define MAXLINE 4096
#define MAX_FILENAME 256 // Changed from 64 bytes

// KEYS
#define ENTER 10
#define KEY_TAB 9

#define TAB_WIDTH 4

#define DISPLAY_DOLLAR_AS_NEWLINE 1
#define TILDE_IN_BLANK_LINE 0

#define scpy(dst, src) strncpy((dst), (src), (sizeof(dst)) - 1)
