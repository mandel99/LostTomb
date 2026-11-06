#ifndef RESTORE_LEGACY_MEMBER_CALLS_H
#define RESTORE_LEGACY_MEMBER_CALLS_H
#include <stdint.h>
/* The original 5614f7/5614fe tail-call thunks accept different stack arities.
   Make the arity explicit instead of guessing it in a variadic trampoline. */
void restore_call_member0(void *object, uintptr_t target);
void restore_call_member1(void *object, uintptr_t target, uintptr_t value);
void restore_call_member2(void *object, uintptr_t target, uintptr_t value, uintptr_t virtual_bases);
#endif
