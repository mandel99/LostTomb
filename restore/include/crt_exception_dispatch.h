#ifndef RESTORE_CRT_EXCEPTION_DISPATCH_H
#define RESTORE_CRT_EXCEPTION_DISPATCH_H

#include <stdint.h>
#include <stddef.h>
#include <intrin.h>
#include "crt_exception_runtime.h"

#if !defined(_MSC_VER) || !defined(_M_IX86)
#error The original exception registration requires MSVC i386 FS:[0].
#endif

/* The registration installed by 56158A is five actual DWORDs. Its final
   three fields are read by 5615DE at registration+8, +12 and +16. */
typedef struct RestoreCrtRegistration5 {
    uint32_t previous;
    uint32_t handler;
    uint32_t descriptor;
    uint32_t frame;
    uint32_t depth;
} RestoreCrtRegistration5;

typedef char RestoreCrtRegistration5Layout[
    (sizeof(void *) == 4 && sizeof(RestoreCrtRegistration5) == 20 &&
     offsetof(RestoreCrtRegistration5, handler) == 4 &&
     offsetof(RestoreCrtRegistration5, descriptor) == 8 &&
     offsetof(RestoreCrtRegistration5, frame) == 12 &&
     offsetof(RestoreCrtRegistration5, depth) == 16) ? 1 : -1];

static __inline uint32_t restore_crt_eh_read32(uint32_t address)
{
    return *(volatile const uint32_t *)(uintptr_t)address;
}

static __inline int32_t restore_crt_eh_read_s32(uint32_t address)
{
    return *(volatile const int32_t *)(uintptr_t)address;
}

uint32_t restore_crt_invoke_registered(uint32_t frame, uint32_t descriptor,
    uint32_t handler, uint32_t depth, uint32_t code);
uint32_t restore_crt_registration_handler(uint32_t exception,
    uint32_t registration, uint32_t context);
uint32_t FUN_005615de(uint32_t exception, uint32_t registration,
    uint32_t context);
void restore_crt_search_handlers(uint32_t exception, uint32_t frame,
    uint32_t context, uint32_t reserved, uint32_t descriptor,
    uint8_t must_catch, uint32_t depth, uint32_t registration);

/* 56389A already has the correct complete C algorithm and remains its
   canonical owner. These types spell the existing x86 stack contract. */
uint32_t FUN_0056389a(int32_t *exception, uint32_t frame, uint32_t context,
    uint32_t reserved, int32_t *descriptor, int32_t depth,
    uint32_t registration, uint32_t must_catch);
void FUN_00563844(void);
int32_t FUN_0056172c(int32_t descriptor, int32_t depth, int32_t state,
    uint32_t *first, uint32_t *limit);
uint8_t FUN_0056876f(void *address, uintptr_t size);
uint32_t FUN_00563b84(uint8_t *handler, uint8_t *conversion,
    uint32_t *throw_info);
void FUN_00563c95(uint32_t exception, int32_t frame, uint32_t context,
    uint32_t reserved, uint32_t descriptor, int32_t handler,
    int32_t conversion, uint32_t *try_block, uint32_t depth,
    int32_t registration);
void FUN_00563adc(uint32_t exception, uint32_t frame, uint32_t context,
    uint32_t reserved, uint32_t descriptor, int32_t state,
    uint32_t depth, uint32_t registration);
/* The 56400A body reads the exception. Its embedded EH3 filter reads
   the low byte of mode: nonzero terminates if the destructor throws. */
void FUN_0056400a(int32_t exception, uint32_t mode);

#endif
