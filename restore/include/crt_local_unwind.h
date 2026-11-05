#ifndef RESTORE_CRT_LOCAL_UNWIND_H
#define RESTORE_CRT_LOCAL_UNWIND_H
#include "crt_exception_runtime.h"
#include <intrin.h>
#include <stddef.h>
typedef struct RestoreCrtUnwindGuard {
 uint32_t previous;
 uint32_t handler;
 uint32_t level;
 uint32_t registration;
} RestoreCrtUnwindGuard;
typedef char RestoreCrtUnwindGuardLayout[(sizeof(RestoreCrtUnwindGuard)==16 && offsetof(RestoreCrtUnwindGuard,registration)==12)?1:-1];
uint32_t restore_crt_local_unwind(uint32_t registration,int32_t stop,uint32_t parent_frame);
uint32_t restore_crt_unwind_guard(EXCEPTION_RECORD *,uint32_t,void *,uint32_t *);
uint32_t FUN_005617c8(EXCEPTION_RECORD *,uint32_t,void *,uint32_t *);
int32_t restore_crt_abnormal_termination(void);
uint32_t restore_crt_notify_frame(uint32_t destination,uint32_t parent_frame);
void restore_crt_global_unwind(PVOID target_frame);
void restore_crt_run_scope(uint32_t target,uint32_t parent_frame,uint32_t entry_eax,uint32_t entry_ecx);
#endif
