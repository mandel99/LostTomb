#ifndef RESTORE_CRT_TRANSLATION_GUARD_H
#define RESTORE_CRT_TRANSLATION_GUARD_H
#include "crt_exception_dispatch.h"

/* Native561603 registration at EBP-40; the last three words describe the
   actual suspended translator frame, not a decompiler's synthetic array. */
typedef struct RestoreCrtTranslationGuard {
    uint32_t previous, handler, descriptor, frame, depth, registration;
    uint32_t continuation, saved_sp, saved_bp, unwind_seen;
} RestoreCrtTranslationGuard;
typedef struct RestoreCrtTranslationFrame {
    uint32_t translated, exception, context;
    RestoreCrtTranslationGuard guard;
} RestoreCrtTranslationFrame;
typedef char RestoreCrtTranslationLayout[
    (sizeof(RestoreCrtTranslationGuard)==40 && sizeof(RestoreCrtTranslationFrame)==52 &&
     offsetof(RestoreCrtTranslationFrame,guard)==12 &&
     offsetof(RestoreCrtTranslationGuard,continuation)==24 &&
     offsetof(RestoreCrtTranslationGuard,saved_sp)==28 &&
     offsetof(RestoreCrtTranslationGuard,saved_bp)==32 &&
     offsetof(RestoreCrtTranslationGuard,unwind_seen)==36) ? 1 : -1];

void restore_crt_unwind_exception(uint32_t target,EXCEPTION_RECORD *exception);
/* Physical canonical entry: original callee pops two stack arguments. */
void FUN_00561505(void);
uint32_t restore_crt_translate_exception(uint32_t *exception,uint32_t frame,
    uint32_t context,uint32_t reserved,uint32_t descriptor,uint32_t depth,
    uint32_t registration);
uint32_t FUN_00561603(uint32_t *exception,uint32_t frame,uint32_t context,
    uint32_t reserved,uint32_t descriptor,uint32_t depth,uint32_t registration);
uint32_t restore_crt_translation_guard_handler(uint32_t exception,
    uint32_t registration,uint32_t context);
uint32_t FUN_005616b7(uint32_t exception,uint32_t registration,uint32_t context);
#endif
