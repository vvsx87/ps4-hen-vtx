#pragma once

#ifndef LIBC_H
#define LIBC_H

#include "types.h"

typedef struct DIR DIR;
typedef int FILE;

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

extern void *(*malloc)(size_t size);
extern void (*free)(void *ptr);
extern void *(*calloc)(size_t num, size_t size);
extern void *(*realloc)(void *ptr, size_t size);
extern void *(*memalign)(size_t boundary, size_t size);
extern void *(*memset)(void *destination, int value, size_t num);
extern void *(*memcpy)(void *destination, const void *source, size_t num);
extern int (*memcmp)(const void *s1, const void *s2, size_t n);
extern void *(*memmove)(void *dst, const void *src, size_t len);
extern errno_t (*memmove_s)(void *dest, rsize_t destsz, const void *src, rsize_t count);
extern char *(*strcpy)(char *destination, const char *source);
extern char *(*strncpy)(char *destination, const char *source, size_t num);
extern errno_t *(*strncpy_s)(char *restrict dest, rsize_t destsz, const char *restrict src, rsize_t count);
extern char *(*strcat)(char *dest, const char *src);
extern char *(*strncat)(char *dest, const char *src, size_t n);
extern size_t (*strlen)(const char *s);
extern int (*strcmp)(const char *s1, const char *s2);
extern int (*strncmp)(const char *s1, const char *s2, size_t n);
extern int (*sprintf)(char *str, const char *format, ...);
extern int (*snprintf)(char *str, size_t size, const char *format, ...);
extern int (*snprintf_s)(char *restrict buffer, rsize_t bufsz, const char *restrict format, ...);
extern int (*sscanf)(const char *str, const char *format, ...);
extern int (*strtol)(const char* s1, char** s2, int base);
extern char *(*strtok)(char *str, const char *delimiters);
extern char *(*strchr)(const char *s, int c);
extern char *(*strrchr)(const char *s, int c);
extern char *(*strstr)(char *str1, char *str2);
extern char *(*strdup)(const char *s);
extern char *(*strtok)(char *str, const char *sep);
extern char *(*index)(const char *s, int c);
extern char *(*rindex)(const char *s, int c);
extern char *(*rindex)(const char *s, int c);
extern int (*isdigit)(int c);
extern int (*atoi)(const char *s);
extern double (*atof)(const char *s);
extern size_t (*strlcpy)(char *dst, const char *src, size_t size);
extern char *(*strerror)(int errnum);
extern void *(*_Getpctype)();
extern unsigned long (*_Stoul)(const char *, char **, int);
extern void (*bcopy)(const void *s1, void *s2, size_t n);
extern double (*ceil)(double x);

extern void (*srand)(unsigned int seed);
extern int (*rand)(void);

extern char *(*asctime)(const struct tm *tm);
extern char *(*asctime_r)(const struct tm *tm, char *buf);
extern char *(*ctime)(const time_t *timep);
extern char *(*ctime_r)(const time_t *timep, char *buf);
extern time_t (*time)(time_t *tloc);
extern struct tm *(*gmtime)(const time_t *timep);
extern struct tm *(*gmtime_s)(const time_t *timep, struct tm *result);
extern struct tm *(*localtime)(const time_t *timep);
extern struct tm *(*localtime_r)(const time_t *timep, struct tm *result);
extern time_t (*mktime)(struct tm *tm);

extern DIR *(*opendir)(const char *filename);
extern struct dirent *(*readdir)(DIR *dirp);
extern int (*readdir_r)(DIR *dirp, struct dirent *entry, struct dirent **result);
extern long (*telldir)(const DIR *dirp);
extern void (*seekdir)(DIR *dirp, long loc);
extern void (*rewinddir)(DIR *dirp);
extern int (*closedir)(DIR *dirp);
extern int (*dirfd)(DIR *dirp);
extern char *(*getprogname)();

extern FILE *(*fopen)(const char *filename, const char *mode);
extern size_t (*fread)(void *ptr, size_t size, size_t count, FILE *stream);
extern size_t (*fwrite)(const void *ptr, size_t size, size_t count, FILE *stream);
extern int (*fseek)(FILE *stream, long int offset, int origin);
extern long int (*ftell)(FILE *stream);
extern int (*fclose)(FILE *stream);
extern int (*fprintf)(FILE *stream, const char *format, ...);

int memset_s(void *s, rsize_t smax, int c, rsize_t n);

void initLibc(void);

#endif
