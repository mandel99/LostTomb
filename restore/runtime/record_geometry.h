#ifndef RESTORE_RECORD_GEOMETRY_H
#define RESTORE_RECORD_GEOMETRY_H
#include <stdint.h>
#include <stdio.h>

/* The original record contains thirteen little-endian 32-bit words. */
void *restore_004e5140(void *destination,const void *source);
uint32_t restore_004e5190(void *record,uint32_t scale,uint32_t step);
void restore_004e52b0(void *records,int count,const volatile uint32_t *scale,const volatile uint32_t *step);

typedef void *(__fastcall *RestoreCopyRecord)(void *,void *,const void *);
typedef uint32_t (__fastcall *RestoreUpdateRecord)(void *,void *);
typedef void (__stdcall *RestoreUpdateRecords)(void *,int);
int restore_verify_geometry(FILE *log,RestoreCopyRecord copy,RestoreUpdateRecord update,
    RestoreUpdateRecords batch,const char *phase);
void restore_geometry_fixtures_close(void);
#endif
