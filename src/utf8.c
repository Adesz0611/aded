/*
 *  (C) 2020 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#include "utf8.h"

size_t utf8_strlen(char *str) {
    size_t i = 0, len = 0;
    while(str[i]) {
        if (!((str[i] & 0xc0) == 0x80))
            ++len;
        ++i;
    }
    return len;
}
