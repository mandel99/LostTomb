#include "crt_local_unwind.h"

/* Stack/continuation primitive: the TargetIp is an actual relocated label,
   replacing the obsolete literal address into the original executable. */
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) void restore_crt_global_unwind(PVOID target_frame)
{
 __asm {
  push ebp
  mov ebp,esp
  push ebx
  push esi
  push edi
  push ebp
  push 0
  push 0
  push offset unwind_resumed
  push [ebp+8]
  call RtlUnwind
 unwind_resumed:
  pop ebp
  pop edi
  pop esi
  pop ebx
  mov esp,ebp
  pop ebp
  ret
 }
}
#pragma warning(pop)

uint32_t restore_crt_unwind_guard(EXCEPTION_RECORD *exception,
 uint32_t registration,void *context,uint32_t *dispatcher)
{
 (void)context;
 if((exception->ExceptionFlags&6u)!=0){
  *dispatcher=registration;
  return 3u;
 }
 return 1u;
}
