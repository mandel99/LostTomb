#ifndef RESTORE_CORE_HELPERS_H
#define RESTORE_CORE_HELPERS_H
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

uint32_t restore_0056233e(uint32_t seed);
uint32_t restore_00562348(void);
char *restore_005632a0(const char *text,const char *pattern);
void *restore_00563400(void *destination,const void *source,size_t count);
typedef uint32_t (__cdecl *RestoreSeed)(uint32_t);
typedef uint32_t (__cdecl *RestoreRandom)(void);
typedef char *(__cdecl *RestoreFind)(const char *,const char *);
typedef void *(__cdecl *RestoreMove)(void *,const void *,size_t);
int restore_verify_core(FILE *log,RestoreSeed seed,RestoreRandom random,RestoreFind find,RestoreMove move,const char *phase);
void restore_core_fixtures_close(void);
#endif
