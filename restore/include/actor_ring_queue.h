#ifndef RESTORE_ACTOR_RING_QUEUE_H
#define RESTORE_ACTOR_RING_QUEUE_H
#include <stdint.h>
#include <stddef.h>
/* Known220-byte prefix. The game's224-byte array stride retains an extra
   untouched DWORD. Methods also accept unaligned byte-addressed objects. */
typedef struct RestoreActorRingQueue {
    uint32_t state,head,tail,capacity,reserved10;
    uint32_t entries[50];
} RestoreActorRingQueue;
typedef char restore_actor_ring_entries_offset[offsetof(RestoreActorRingQueue,entries)==20?1:-1];
typedef char restore_actor_ring_known_size[sizeof(RestoreActorRingQueue)==220?1:-1];
uint32_t restore_actor_ring_initialize(void *);
uint32_t restore_actor_ring_clear(void *);
uint32_t restore_actor_ring_contains(const void *,uint32_t);
uint32_t restore_actor_ring_push(void *,const void *);
uint32_t restore_actor_ring_next(const void *,void *);
uint32_t restore_actor_ring_pop(void *);
uint32_t restore_actor_ring_count(const void *);
uint32_t restore_actor_ring_full(const void *);
void *restore_actor_ring_construct(void *);
uint32_t restore_actor_ring_destroy(void *);
void FUN_004f64b0(void);
int FUN_004f64d0(void);
int FUN_004f64a0(void);
void FUN_004f64e0(void);
/* Generated legacy C callers use cdecl cleanup; capture ECX before C. */
#define RESTORE_RING_ENTRY0(name,body) \
 __declspec(naked) void name(void) { \
 __asm { push ecx } __asm { call body } __asm { add esp,4 } __asm { ret } }
#define RESTORE_RING_ENTRY1(name,body) \
 __declspec(naked) void name(void) { \
 __asm { push dword ptr[esp+4] } __asm { push ecx } \
 __asm { call body } __asm { add esp,8 } __asm { ret } }
#endif
