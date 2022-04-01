#ifndef __NOSTDLIB_CSTRING_H__
#define __NOSTDLIB_CSTRING_H__
#include <cstddef.h>

namespace std {
extern char *strcpy(char *__restrict dest, const char *__restrict src);
extern char *strncpy(char *__restrict dest, const char *__restrict src,
                     size_t n);

extern char *strcat(char *__restrict dest, const char *__restrict src);
extern char *strncat(char *__restrict dest, const char *__restrict src,
                     size_t n);

extern size_t strxfrm(char *__restrict dest, const char *__restrict src,
                      size_t n);

[[gnu::malloc]] extern char *strdup(const char *s);
[[gnu::malloc]] extern char *strndup(const char *string, size_t n);

extern size_t strlen(const char *s);

extern int strcmp(const char *s1, const char *s2);
extern int strncmp(const char *s1, const char *s2, size_t n);

extern int strcoll(const char *s1, const char *s2);

extern char *strchr(char *s, int c);
extern const char *strchr(const char *s, int c);

extern char *strrchr(char *s, int c);
extern const char *strrchr(const char *s, int c);

extern size_t strspn(const char *s, const char *accept);
extern size_t strcspn(const char *s, const char *reject);

extern char *strpbrk(char *s, const char *accept);
extern const char *strpbrk(const char *s, const char *accept);

extern char *strstr(char *s, const char *target);
extern const char *strstr(const char *s, const char *target);

extern char *strtok(char *__restrict s, const char *__restrict delim);

extern void *memchr(void *s, int c, size_t n);
extern const void *memchr(const void *s, int c, size_t n);

extern int memcmp(const void *s1, const void *s2, size_t n);
extern void *memset(void *s, int c, size_t n);
extern void *memcpy(void *__restrict dest, const void *__restrict src,
                    size_t n);
extern void *memccpy(void *__restrict dest, const void *__restrict src, int c,
                     size_t n);
extern void *memmove(void *dest, const void *src, size_t n);
} // namespace std

#endif
