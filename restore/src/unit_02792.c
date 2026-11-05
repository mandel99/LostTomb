#include "crt_local_unwind.h"

/* The metadata-only MASM object registers FUN_005617c8 as a SafeSEH target. */
#pragma warning(push)
#pragma warning(disable:4733)
uint32_t restore_crt_local_unwind(uint32_t registration,int32_t stop,uint32_t parent_frame)
{
 volatile RestoreCrtUnwindGuard guard;
 guard.registration=registration;
 guard.level=0xfffffffeu;
 guard.handler=(uint32_t)(uintptr_t)FUN_005617c8;
 guard.previous=__readfsdword(0);
 _ReadWriteBarrier();
 __writefsdword(0,(uint32_t)(uintptr_t)&guard);
 _ReadWriteBarrier();
 for(;;){
  uint32_t table=*(volatile uint32_t *)(uintptr_t)(registration+8u);
  uint32_t level=*(volatile uint32_t *)(uintptr_t)(registration+12u);
  uint32_t scope,next;
  if(level==0xffffffffu || level==(uint32_t)stop)break;
  scope=table+level*12u;
  next=*(volatile uint32_t *)(uintptr_t)scope;
  guard.level=next;
  *(volatile uint32_t *)(uintptr_t)(registration+12u)=next;
  if(*(volatile uint32_t *)(uintptr_t)(scope+4u)==0){
   uint32_t notified=*(volatile uint32_t *)(uintptr_t)(scope+8u);
   uint32_t target;
   /* Native pushes101 but the shared Notify1 entry reads [parent EBP+8]. */
   restore_crt_notify_frame(notified,parent_frame);
   target=*(volatile uint32_t *)(uintptr_t)(scope+8u);
   restore_crt_run_scope(target,parent_frame,notified,next);
  }
 }
 _ReadWriteBarrier();
 __writefsdword(0,guard.previous);
 _ReadWriteBarrier();
 /* Native's last loop-head load leaves the actual registration in EAX. */
 return registration;
}
#pragma warning(pop)
