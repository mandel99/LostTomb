#include "crt_eh3_frames.h"

/* All installed handlers are listed in the linked SafeSEH metadata object. */
#pragma warning(push)
#pragma warning(disable:4733)
void restore_crt_eh3_install(volatile RestoreCrtEh3Registration *record,
 const RestoreCrtEh3Scope *scopes)
{
 record->state=-1;
 record->scopes=scopes;
 record->handler=(uint32_t)(uintptr_t)FUN_00561a90;
 record->previous=__readfsdword(0);
 _ReadWriteBarrier();
 __writefsdword(0,(uint32_t)(uintptr_t)record);
 _ReadWriteBarrier();
}
void restore_crt_eh3_restore(const volatile RestoreCrtEh3Registration *record)
{
 _ReadWriteBarrier();
 __writefsdword(0,record->previous);
 _ReadWriteBarrier();
}
#pragma warning(pop)

__declspec(noreturn) void FUN_005637ee(void);
int32_t restore_crt_cleanup_exception_filter(EXCEPTION_POINTERS *pointers)
{
 if(pointers->ExceptionRecord->ExceptionCode==0xe06d7363u)FUN_005637ee();
 return 0;
}
int32_t FUN_00563c7f(EXCEPTION_POINTERS *pointers)
{return restore_crt_cleanup_exception_filter(pointers);}

int32_t restore_crt_rethrow_filter(EXCEPTION_POINTERS *pointers)
{
 const volatile uint32_t *exception=(const volatile uint32_t *)pointers->ExceptionRecord;
 return exception[0]==0xe06d7363u && exception[4]==3u &&
  exception[5]==0x19930520u && exception[7]==0u ? 1 : 0;
}
int32_t FUN_00563e1c(EXCEPTION_POINTERS *pointers)
{return restore_crt_rethrow_filter(pointers);}
