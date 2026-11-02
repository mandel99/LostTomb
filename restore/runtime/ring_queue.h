#ifndef RESTORE_RING_QUEUE_H
#define RESTORE_RING_QUEUE_H
#include <stdint.h>
#include <stdio.h>

uint32_t restore_004f6520(void *queue);
uint32_t restore_004f6560(void *queue);
uint32_t restore_004f6570(const void *queue,uint32_t value);
uint32_t restore_004f65b0(void *queue,const void *value);
uint32_t restore_004f65f0(const void *queue,void *cursor);
uint32_t restore_004f6630(const void *queue);

/* fastcall's unused EDX slot adapts the original thiscall ECX and stack. */
typedef uint32_t (__fastcall *RestoreQueueOne)(void *,void *);
typedef uint32_t (__fastcall *RestoreQueueFind)(const void *,void *,uint32_t);
typedef uint32_t (__fastcall *RestoreQueuePush)(void *,void *,const void *);
typedef uint32_t (__fastcall *RestoreQueueNext)(const void *,void *,void *);
typedef struct RestoreQueueFunctions {
    RestoreQueueOne init,clear;
    RestoreQueueFind find;
    RestoreQueuePush push;
    RestoreQueueNext next;
    RestoreQueueOne count;
} RestoreQueueFunctions;
int restore_verify_queue(FILE *log,const RestoreQueueFunctions *functions,const char *phase);
void restore_queue_fixtures_close(void);
#endif
