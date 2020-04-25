#pragma once
#define MAXLINE 4096
#define MAX_FILENAME 256 // Changed from 64 bytes

// KEYS
#define ENTER 10
#define KEY_TAB 9

#define TAB_WIDTH 4

#define scpy(dst, src) strncpy((dst), (src), (sizeof(dst)) - 1)
