#include "crt_eh3_frames.h"
#include "legacy_member_calls.h"

__declspec(noreturn) void FUN_005637ee(void);
uint32_t restore_crt_destroy_exception_recover(void);
static int32_t destroy_exception_filter_mode(uint32_t mode)
{return (uint8_t)mode!=0;}
/* The second argument is consumed by the filter, even though the body does
   not read it. Keep its actual parent-frame slot across destructor callbacks. */
__declspec(naked) static int32_t restore_crt_destroy_exception_filter(void)
{
 __asm push dword ptr [ebp+12]
 __asm call destroy_exception_filter_mode
 __asm add esp,4
 __asm ret
}
const RestoreCrtEh3Scope restore_crt_destroy_exception_scopes[1]={
 {-1,restore_crt_destroy_exception_filter,restore_crt_destroy_exception_recover}
};
static void prepare_destroy_exception(volatile RestoreCrtEh3Frame24 *f,
 const volatile uint32_t *arguments,uint32_t saved_sp)
{
 (void)arguments;
 restore_crt_eh3_install(&f->registration,restore_crt_destroy_exception_scopes);
 f->saved_sp=saved_sp;
}
static void run_destroy_exception(volatile RestoreCrtEh3Frame24 *f,
 const volatile uint32_t *arguments,uint32_t parent_frame,uint32_t recovered)
{
 uint32_t exception=arguments[0],destructor;
 (void)parent_frame;(void)recovered;
 if(exception && (destructor=restore_crt_eh_read32(restore_crt_eh_read32(exception+28)+4))!=0){
  f->registration.state=0;
  restore_call_member0((void *)(uintptr_t)restore_crt_eh_read32(exception+24),destructor);
  f->registration.state=-1;
 }
 restore_crt_eh3_restore(&f->registration);
}
RESTORE_EH3_PARENT(restore_crt_destroy_exception,void,
 (int32_t exception,uint32_t mode),24,prepare_destroy_exception,run_destroy_exception)
__declspec(naked) uint32_t restore_crt_destroy_exception_recover(void)
{__asm mov esp,[ebp-24] __asm jmp FUN_005637ee}
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) void FUN_0056400a(int32_t exception,uint32_t mode)
{__asm jmp restore_crt_destroy_exception}

#pragma warning(pop)
