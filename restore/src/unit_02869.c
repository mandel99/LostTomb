#include "crt_exception_runtime.h"

/* This edge supplies the parent EBP expected by compiler-generated funclets.
   Capture EBP after the callback, before restoring the C caller's frame. */
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static uint32_t __cdecl restore_crt_call_frame(
 uint32_t handler,uint32_t frame,uint32_t code,uint32_t *returned_frame)
{
 __asm {
  push ebp
  push ebx
  push esi
  push edi
  mov ebp,[esp+24]
  mov ecx,[esp+28]
  mov eax,[esp+20]
  call eax
  mov ecx,[esp+32]
  mov [ecx],ebp
  pop edi
  pop esi
  pop ebx
  pop ebp
  ret
 }
}
#pragma warning(pop)

uint32_t restore_crt_invoke_frame(uint32_t handler,uint32_t frame,uint32_t code)
{
 uint32_t returned_frame;
 uint32_t callback_frame=frame+12u;
 uint32_t result;
 restore_crt_notify_jump(handler,code,callback_frame);
 result=restore_crt_call_frame(handler,callback_frame,code,&returned_frame);
 restore_crt_notify_jump(result,code==0x100u?2u:code,returned_frame);
 return result;
}
