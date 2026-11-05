#include "crt_exception_runtime.h"

/* The existing predicate returns unsigned char, matching restore_types bool. */
uint8_t FUN_005687a7(FARPROC candidate);
#define RESTORE_PREVIOUS_FILTER (*(LPTOP_LEVEL_EXCEPTION_FILTER volatile *)(uintptr_t)0x012666a4u)

int32_t restore_crt_unhandled_filter(EXCEPTION_POINTERS *exception)
{
 EXCEPTION_RECORD *record=exception->ExceptionRecord;
 LPTOP_LEVEL_EXCEPTION_FILTER candidate;
 if(record->ExceptionCode==0xe06d7363u && record->NumberParameters==3u &&
    record->ExceptionInformation[0]==0x19930520u)
  restore_crt_terminate();
 candidate=RESTORE_PREVIOUS_FILTER;
 if(candidate!=0 && FUN_005687a7((FARPROC)candidate)!=0){
  /* The validator can change this global: native reloads the actual target. */
  return (int32_t)RESTORE_PREVIOUS_FILTER(exception);
 }
 return 0;
}
