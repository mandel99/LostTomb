#include "crt_runtime_init.h"
#include "host_allocator.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define PTR(a) ((void *)(uintptr_t)(uint32_t)(a))

/* 5618E6 returns the actual callback value, including zero, or zero on
   allocation failure. Re-read globals at the native callback boundaries. */
uint32_t restore_crt_append_exit_callback(uint32_t callback)
{
 uint32_t capacity,base,next,replacement,bytes,offset;
 capacity=(uint32_t)FUN_005647c2(PTR(U(0x1266a8cu)));
 base=U(0x1266a8cu);next=U(0x1266a88u);
 if(capacity<next-base+4u){
  bytes=(uint32_t)FUN_005647c2(PTR(base))+16u;
  replacement=(uint32_t)(uintptr_t)FUN_005645a0(PTR(U(0x1266a8cu)),bytes);
  if(replacement==0)return 0;
  offset=U(0x1266a88u)-U(0x1266a8cu);
  U(0x1266a8cu)=replacement;
  /* Native SAR offset,2 followed by LEA *4 retains all bits above bit1. */
  next=replacement+(offset&0xfffffffcu);
  U(0x1266a88u)=next;
 }
 U(next)=callback;
 /* Native ADD reloads the live global after writing the callback slot. */
 U(0x1266a88u)=U(0x1266a88u)+4u;
 return callback;
}
uint32_t FUN_005618e6(uint32_t callback)
{return restore_crt_append_exit_callback(callback);}
