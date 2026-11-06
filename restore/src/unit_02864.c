#include "crt_eh3_frames.h"

uint32_t restore_crt_catch_recover(void);
uint32_t restore_crt_catch_finally(void);
__declspec(naked) static int32_t restore_crt_eh3_catch_filter(void)
{
 __asm {
  push [ebp-20]
  call FUN_00563e1c
  add esp,4
  ret
 }
}
const RestoreCrtEh3Scope restore_crt_catch_scopes[2]={
 {-1,0,restore_crt_catch_finally},
 {0,restore_crt_eh3_catch_filter,restore_crt_catch_recover}
};
static void prepare_eh3_catch(volatile RestoreCrtEh3CatchFrame *f,
 const volatile uint32_t *arguments,uint32_t saved_sp)
{
 restore_crt_eh3_install(&f->registration,restore_crt_catch_scopes);
 f->saved_sp=saved_sp;
 f->result=arguments[4];
 f->skip_release=0;
 f->saved_exception_cell=restore_crt_eh_read32(arguments[1]-4u);
 f->old_exception=restore_crt_eh_read32(0x01266418u);
 f->old_context=restore_crt_eh_read32(0x0126641cu);
 *(volatile uint32_t *)0x01266418u=arguments[0];
 *(volatile uint32_t *)0x0126641cu=arguments[2];
}
static uint32_t run_eh3_catch(volatile RestoreCrtEh3CatchFrame *f,
 const volatile uint32_t *arguments,uint32_t parent_frame,uint32_t recovered)
{
 if(recovered){
  f->result=0;
  (void)restore_crt_local_unwind((uint32_t)(uintptr_t)&f->registration,-1,parent_frame);
 }else{
  uint32_t result,frame=arguments[1],exception=arguments[0];
  f->registration.state=0;
  f->registration.state=1;
  result=restore_crt_invoke_registered(frame,arguments[3],arguments[4],arguments[5],arguments[6]);
  f->result=result;
  f->registration.state=0;
  f->registration.state=-1;
  restore_crt_catch_cleanup(parent_frame,frame,exception,0);
 }
 restore_crt_eh3_restore(&f->registration);
 return f->result;
}
RESTORE_EH3_PARENT(restore_crt_invoke_catch,uint32_t,
 (uint32_t exception,uint32_t frame,uint32_t context,uint32_t descriptor,
 uint32_t handler,uint32_t depth,uint32_t code),44,prepare_eh3_catch,run_eh3_catch)
RESTORE_EH3_RECOVERY(restore_crt_catch_recover,44,run_eh3_catch)
__declspec(naked) uint32_t restore_crt_catch_finally(void)
{
 __asm {
  push 0
  push [ebp+8]
  push [ebp+12]
  push ebp
  call restore_crt_catch_cleanup
  add esp,16
  ret
 }
}
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) uint32_t FUN_00563d10(uint32_t exception,uint32_t frame,
 uint32_t context,uint32_t descriptor,uint32_t handler,uint32_t depth,uint32_t code)
{__asm jmp restore_crt_invoke_catch}
#pragma warning(pop)
