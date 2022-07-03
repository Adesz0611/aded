/*
 *  (C) 2021 Adam Hunyadvari
 *      <adesz@jss.hu>
 */
#include <stdio.h>

#include "utf8.h"

/* Function that returns the length of an utf8 string */
size_t utf8_strlen(const char *str) {
    size_t i = 0, len = 0;
    while(str[i]) {
        if(isutf8(str[i]))
            ++len;
        ++i;
    }
    return len;
}

size_t utf8_iterate_forward(const char *str, size_t pos, size_t n) {
    size_t i = 0, ans = pos;
    while(str[ans] && i < n) {
        ans++;
        if(isutf8(str[ans]))
            i++;
    }
    return ans;
}

size_t utf8_iterate_backward(const char *str, size_t pos, size_t n) {
    size_t i = 0, ans = pos;
    while(str[ans] && i < n) {
        ans--;
        if(isutf8(str[ans]))
            i++;
    }
    return ans;
}
