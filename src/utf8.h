/*
 *  (C) 2021 Adam Hunyadvari
 *      <adesz@jss.hu>
 */

#ifndef UTF8_H
#define UTF8_H


#define isutf8(c) (((c)&0xC0)!=0x80)

size_t utf8_strlen(const char *str);
size_t utf8_iterate_forward(const char *str, size_t pos, size_t n);
size_t utf8_iterate_backward(const char *str, size_t pos, size_t n);

#endif
