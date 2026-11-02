#ifndef RESTORE_MONUMENT_CONSTRUCTOR_H
#define RESTORE_MONUMENT_CONSTRUCTOR_H
#include <stdint.h>
typedef struct RestoreMonumentRecord {uint32_t fields[13];} RestoreMonumentRecord;
/* The native temporary initializes four of thirteen DWORDs. The body API
   makes the other nine incoming words explicit; public C wrappers use zero.
   Startup geometry subsequently derives every piece's remaining fields. */
typedef struct RestoreMonumentConstructorScratch {
    RestoreMonumentRecord base,part;
} RestoreMonumentConstructorScratch;
void restore_monument_class_base(void *object,RestoreMonumentRecord *temporary,int large);
void restore_monument_class_seed(void *object,uint32_t class_index,RestoreMonumentRecord *temporary);
void restore_monument_class_clear(void *object,uint32_t class_index);
void *restore_monument_zero_record(void *object);
void *restore_monument_copy_record(void *destination,const void *source);
void restore_monument_constructors_initialize(void);
void *FUN_0040d280(void *object),*FUN_00401030(void *object);
void *FUN_00416c20(void *object),*FUN_004e6400(void *object);
void *FUN_005430e0(void *object),*FUN_005567d0(void *object);
void *FUN_0054fef0(void *object),*FUN_005480e0(void *object);
void *FUN_004e5210(void *object),*FUN_00545f80(void *object);
void *FUN_004e5140(void *destination,const void *source);
void *FUN_004e5120(void *object,int32_t x,int32_t y,int32_t size,int32_t type);
#endif
