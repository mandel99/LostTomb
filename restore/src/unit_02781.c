#include "crt_eh3_frames.h"

#pragma warning(push)
#pragma warning(disable:4733)
static void unlink_exception_head(void)
{
 uint32_t head=__readfsdword(0);
 uint32_t previous=*(volatile uint32_t *)(uintptr_t)head;
 _ReadWriteBarrier();
 __writefsdword(0,previous);
 _ReadWriteBarrier();
}
#pragma warning(pop)

/* A catch continuation is a nonlocal JMP. Preserve real incoming ECX/EDX;
   the last ADD reproduces the native flags from frame+12 before transfer. */
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) __declspec(noreturn) void restore_crt_jump_to_catch(
 uint32_t destination,uint32_t frame)
{
 __asm {
  push ecx
  push edx
  call unlink_exception_head
  pop edx
  pop ecx
  mov eax,[esp+4]
  mov ebx,[esp+8]
  mov ebp,ebx
  add ebp,12
  mov esp,[ebx-4]
  jmp eax
 }
}
__declspec(naked) __declspec(noreturn) void FUN_005614c3(
 uint32_t destination,uint32_t frame)
{
 __asm jmp restore_crt_jump_to_catch
}
#pragma warning(pop)
