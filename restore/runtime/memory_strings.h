#ifndef RESTORE_MEMORY_STRINGS_H
#define RESTORE_MEMORY_STRINGS_H
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

char *restore_00562240(char *destination,const char *source,size_t count);
int restore_005631b0(const char *left,const char *right);
void *restore_00563240(void *destination,int value,size_t count);
size_t restore_00563380(const char *text);

typedef char *(__cdecl *RestoreBoundedStringCopy)(char *,const char *,size_t);
typedef int (__cdecl *RestoreStringCompare)(const char *,const char *);
typedef void *(__cdecl *RestoreMemoryFill)(void *,int,size_t);
typedef size_t (__cdecl *RestoreStringLength)(const char *);
int restore_verify_memory_strings(FILE *log,RestoreBoundedStringCopy copy,RestoreStringCompare compare,
    RestoreMemoryFill fill,RestoreStringLength length,const char *phase);
void restore_memory_string_fixtures_close(void);
#endif
