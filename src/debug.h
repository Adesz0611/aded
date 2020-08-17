#ifndef DEBUG_H
#define DEBUG_H

#include <stdarg.h>

#define DEBUG_FILE "aded.debug"

#ifdef ALLOW_DEBUG
#define DEBUGF(...) debugf(__VA_ARGS__)
#else
#define DEBUGF(...)
#endif

void debugf (const char *fmt, ...);

#endif
