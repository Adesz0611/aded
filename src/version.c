#include <stdio.h>

#include "version.h"

void version(void)
{
    printf("%s version %s\n", PROGRAM_NAME, VERSION);
    printf("Codename: %s\n", CODENAME);
}
