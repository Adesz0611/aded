#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "version.h"

void print_usage(void)
{
    printf("Usage: %s [options] filename\n", PROGRAM_NAME);
    fputs("Options:\n", stdout);
    fputs("         --help     display help then exit\n", stdout);
    fputs("         --version  display version informations then exit\n", stdout);
}

void print_version(void)
{
    printf("%s version %s\n", PROGRAM_NAME, VERSION);
    printf("Codename: %s\n", CODENAME);
}
