#include "crt_eh3_frames.h"

uint32_t restore_crt_cleanup_recover(void);
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static int32_t restore_crt_eh3_cleanup_filter(void)
{
 __asm {
  push [ebp-20]
  call FUN_00563c7f
  add esp,4
  ret
 }
}
#pragma warning(pop)
const RestoreCrtEh3Scope restore_crt_cleanup_scopes[1]={
 {-1,restore_crt_eh3_cleanup_filter,restore_crt_cleanup_recover}
};
static void prepare_eh3_cleanup(volatile RestoreCrtEh3CleanupFrame *f,
 const volatile uint32_t *arguments,uint32_t saved_sp)
{
 restore_crt_eh3_install(&f->registration,restore_crt_cleanup_scopes);
 f->saved_sp=saved_sp;
 f->current_state=restore_crt_eh_read_s32(arguments[0]+8u);
}
static void run_eh3_cleanup(volatile RestoreCrtEh3CleanupFrame *f,
 const volatile uint32_t *arguments,uint32_t parent_frame,uint32_t recovered)
{
 uint32_t frame=arguments[0],descriptor=arguments[2];
 int32_t state=f->current_state;
 (void)parent_frame;
 if(recovered){
  f->registration.state=-1;
  state=restore_crt_eh_read_s32(restore_crt_eh_read32(descriptor+8u)+(uint32_t)state*8u);
  f->current_state=state;
 }
 while(state!=(int32_t)arguments[3]){
  uint32_t table,handler;
  if(state<0 || state>=restore_crt_eh_read_s32(descriptor+4u))FUN_00563844();
  f->registration.state=0;
  table=restore_crt_eh_read32(descriptor+8u);
  handler=restore_crt_eh_read32(table+(uint32_t)state*8u+4u);
  if(handler) (void)restore_crt_invoke_frame(handler,frame,0x103u);
  f->registration.state=-1;
  table=restore_crt_eh_read32(descriptor+8u);
  state=restore_crt_eh_read_s32(table+(uint32_t)state*8u);
  f->current_state=state;
 }
 *(volatile uint32_t *)(uintptr_t)(frame+8u)=(uint32_t)state;
 restore_crt_eh3_restore(&f->registration);
}
RESTORE_EH3_PARENT(restore_crt_cleanup_to_state,void,
 (uint32_t frame,uint32_t reserved,uint32_t descriptor,int32_t stop),28,
 prepare_eh3_cleanup,run_eh3_cleanup)
RESTORE_EH3_RECOVERY(restore_crt_cleanup_recover,28,run_eh3_cleanup)
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) void FUN_00563be1(uint32_t frame,uint32_t reserved,
 uint32_t descriptor,int32_t stop)
{__asm jmp restore_crt_cleanup_to_state}
#pragma warning(pop)
