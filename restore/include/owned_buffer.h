#ifndef RESTORE_OWNED_BUFFER_H
#define RESTORE_OWNED_BUFFER_H
#include <stdint.h>
typedef struct RestoreOwnedBuffer {uint32_t pointer,owned;} RestoreOwnedBuffer;
RestoreOwnedBuffer *restore_owned_buffer_initialize(RestoreOwnedBuffer *self);
void restore_owned_buffer_release(RestoreOwnedBuffer *self);
RestoreOwnedBuffer *FUN_00560280(RestoreOwnedBuffer *self);
void FUN_005602b0(RestoreOwnedBuffer *self);
void restore_owned_buffer_construct_element(void *self);
void restore_owned_buffer_destroy_element(void *self);
void *restore_owned_buffer_delete(RestoreOwnedBuffer *self,uint32_t flags);
void *FUN_00560210(RestoreOwnedBuffer *self,uint32_t flags);
void restore_owned_buffer_cleanup_frame(uint32_t frame);
void FUN_0056e155(uint32_t frame);
#endif
