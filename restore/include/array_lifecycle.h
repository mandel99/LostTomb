#ifndef RESTORE_ARRAY_LIFECYCLE_H
#define RESTORE_ARRAY_LIFECYCLE_H
#include <stdint.h>
typedef void (*RestoreArrayElement)(void *object);
void restore_array_destroy(void *base,uint32_t stride,int32_t count,RestoreArrayElement destroy);
void restore_array_construct(void *base,uint32_t stride,int32_t count,RestoreArrayElement construct,RestoreArrayElement destroy);
void restore_array_unwind(void *end,uint32_t stride,int32_t count,RestoreArrayElement destroy);
int32_t restore_array_unwind_exception_filter(uint32_t code);
void restore_array_destroy_cleanup_frame(uint32_t frame);
void restore_array_construct_cleanup_frame(uint32_t frame);
void FUN_00561363(void *base,uint32_t stride,int32_t count,RestoreArrayElement destroy);
void FUN_005613e3(void *end,uint32_t stride,int32_t count,RestoreArrayElement destroy);
void FUN_00561441(void *base,uint32_t stride,int32_t count,RestoreArrayElement construct,RestoreArrayElement destroy);
void FUN_005613cb(uint32_t frame);
void FUN_005614ab(uint32_t frame);
#endif
