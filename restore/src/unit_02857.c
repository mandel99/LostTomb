#include "crt_eh3_frames.h"

__declspec(noreturn) void FUN_005637ee(void);
uint32_t restore_crt_terminate_recover(void);
uint32_t restore_crt_terminate_finally(void);
static int32_t restore_crt_eh3_execute_terminate_filter(void){return 1;}
const RestoreCrtEh3Scope restore_crt_terminate_scopes[2]={
 {-1,0,restore_crt_terminate_finally},
 {0,restore_crt_eh3_execute_terminate_filter,restore_crt_terminate_recover}
};
static void prepare_eh3_terminate(volatile RestoreCrtEh3Frame24 *f,
 const volatile uint32_t *arguments,uint32_t saved_sp)
{
 (void)arguments;
 restore_crt_eh3_install(&f->registration,restore_crt_terminate_scopes);
 f->saved_sp=saved_sp;
}
static __declspec(noreturn) void run_eh3_terminate(volatile RestoreCrtEh3Frame24 *f,
 const volatile uint32_t *arguments,uint32_t parent_frame,uint32_t recovered)
{
 void (__cdecl *callback)(void);
 (void)arguments;(void)parent_frame;
 f->registration.state=0;
 if(!recovered){
  callback=*(void (__cdecl *volatile *)(void))0x00604c24u;
  if(callback){f->registration.state=1;callback();f->registration.state=0;}
 }
 f->registration.state=-1;
 FUN_005637ee();
}
RESTORE_EH3_PARENT(restore_crt_invoke_terminate_handler,void,(void),24,
 prepare_eh3_terminate,run_eh3_terminate)
RESTORE_EH3_RECOVERY(restore_crt_terminate_recover,24,run_eh3_terminate)
__declspec(naked) uint32_t restore_crt_terminate_finally(void)
{__asm jmp FUN_005637ee}
__declspec(naked) void FUN_00563844(void)
{__asm jmp restore_crt_invoke_terminate_handler}
