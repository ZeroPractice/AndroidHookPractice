//
// Created by bingghost on 2017/4/7.
//

#ifndef SECURITYLIBRARY_CSTRING_H
#define SECURITYLIBRARY_CSTRING_H

#include <ctype.h>
#include <stddef.h>

namespace cstring {
    /* * * * * * * * * * * * * * * * * * * memory functions * * * * * * * * * * * * * * * * * * * */
    void *memrchr(const void *s, int c, size_t n);

    void memswap(void *m1, void *m2, size_t n);

    void* memset(void*  dst, int c, size_t n);

    void *memchr(const void *s, int c, size_t n);

    int memcmp(const void *s1, const void *s2, size_t n);

    void *memmem(const void *haystack, size_t n, const void *needle, size_t m);

    void *memmove(void *dst, const void *src, size_t n);

    void *memccpy(void *dst, const void *src, int c, size_t n);

    void* memcpy(void *dst0, const void *src0, size_t length);

    /* * * * * * * * * * * * * * * * * * * string functions * * * * * * * * * * * * * * * * * * * */
    char* index(const char *p, int ch);

    size_t strlen(const char *str);

    char* strcat(char *s, const char *append);

    char* strchr(const char *p, int ch);

    int strcmp(const char *s1, const char *s2);

    char* strcpy(char *to, const char *from);

    size_t strlcat(char *dst, const char *src, size_t siz);

    size_t strlcpy(char *dst, const char *src, size_t siz);

    char* strncat(char *dst, const char *src, size_t n);

    int strncmp(const char *s1, const char *s2, size_t n);

    char* strncpy(char *dst, const char *src, size_t n);

    size_t  strnlen(const char*  str, size_t  maxlen);

    char* strpbrk(const char *s1, const char *s2);

    char* strrchr(const char *p, int ch);

    char* strsep(char **stringp, const char *delim);

    size_t strspn(const char *s1, const char *s2);

    char* strstr(const char *s, const char *find);

    char* strcasestr(const char *s, const char *find);

    char* strtok(char *s, const char *delim);

    int strcasecmp(const char *s1, const char *s2);

    int strncasecmp(const char *s1, const char *s2, size_t n);
};


#endif //SECURITYLIBRARY_CSTRING_H
