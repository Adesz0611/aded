#include <stdio.h>
#include <stdarg.h>

#include "debug.h"


void debugf (const char *fmt, ...) {
    FILE *f = fopen(DEBUG_FILE, "a+");
    if (f == NULL) {
        return;
    }

    va_list ap;
    va_start(ap, fmt);
    vfprintf(f, fmt, ap);
    va_end(ap);

    fclose(f);
    return;
}
