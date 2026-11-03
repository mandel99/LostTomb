#ifndef RESTORE_MONUMENT_BASE_QUERIES_H
#define RESTORE_MONUMENT_BASE_QUERIES_H
#include <stdint.h>
void restore_monument_base_initialize(void);
uint8_t restore_monument_base_worker(uint32_t token,uint32_t building);
/* Native worker predicates define AL and retain incoming EAX above it. */
uint32_t restore_monument_base_worker_result(uint32_t token,uint32_t building,uint32_t incoming_eax);
int32_t restore_monument_base_0040d940(void *self,uint32_t building,int32_t *output);
int32_t restore_monument_base_00417050(void *self,uint32_t building,int32_t *output);
int32_t restore_monument_base_004e6710(void *self,uint32_t building,int32_t *output);
/* Native progress members have two stack arguments and an ECX receiver.
   Legacy C entries capture ECX, then use ordinary cdecl caller cleanup. */
#define RESTORE_MONUMENT_QUERY_ENTRY(name,body) \
 __declspec(naked) void name(void) { \
 __asm { push dword ptr[esp+8] } __asm { push dword ptr[esp+8] } \
 __asm { push ecx } __asm { call body } __asm { add esp,12 } __asm { ret } }
#define RESTORE_MONUMENT_WORKER_ENTRY(name,token) \
 __declspec(naked) void name(void) { \
 __asm { push eax } __asm { push dword ptr[esp+8] } __asm { push token } \
 __asm { call restore_monument_base_worker_result } __asm { add esp,12 } __asm { ret } }
#endif
