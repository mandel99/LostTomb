#include "crt_lowio.h"
#include "crt_runtime_tail.h"
#include <stddef.h>

extern void * __cdecl FUN_00561b68(size_t bytes);
typedef char RestoreCrtLowioStartupLayout[(sizeof(void *)==4 &&
 sizeof(STARTUPINFOA)==68 && offsetof(STARTUPINFOA,cbReserved2)==50 &&
 offsetof(STARTUPINFOA,lpReserved2)==52)?1:-1];

/* Reuse both native initialization loops. Reserved bytes6..7 stay untouched;
   after each record the upper bound comes from the actual current table slot. */
static void initialize_lowio_block(RestoreCrtLowioRecord *allocation,
 RestoreCrtLowioRecord *volatile *slot)
{
 volatile RestoreCrtLowioRecord *record=allocation;
 uint32_t limit=(uint32_t)(uintptr_t)allocation+0x100u;
 while((uint32_t)(uintptr_t)record<limit){
  record->flags=0;
  record->handle=0xffffffffu;
  record->text_state=10;
  limit=(uint32_t)(uintptr_t)*slot+0x100u;
  record=(volatile RestoreCrtLowioRecord *)(uintptr_t)
   ((uint32_t)(uintptr_t)record+8u);
 }
}

void restore_crt_initialize_lowio(void)
{
 RestoreCrtLowioRecord *block;
 STARTUPINFOA startup;
 uint32_t count,index,block_index;
 volatile const uint8_t *inherited_flags;
 volatile const uint32_t *inherited_handles;

 block=FUN_00561b68(0x100);
 /* 5630E0 can return through a custom exit callback. Native continues using
    the original allocation result; do not substitute that callback's EAX. */
 if(block==0)(void)FUN_005630e0(27);
 RESTORE_CRT_LOWIO_BLOCKS[0]=block;
 RESTORE_CRT_LOWIO_CAPACITY=32;
 initialize_lowio_block(block,&RESTORE_CRT_LOWIO_BLOCKS[0]);
 /* No invented cb or zero initialization precedes the native output call. */
 GetStartupInfoA(&startup);
 if(startup.cbReserved2!=0 && startup.lpReserved2!=0){
  count=*(volatile const uint32_t *)startup.lpReserved2;
  inherited_flags=(volatile const uint8_t *)(uintptr_t)
   ((uint32_t)(uintptr_t)startup.lpReserved2+4u);
  /* The handle array follows ORIGINAL count bytes, before the signed cap. */
  inherited_handles=(volatile const uint32_t *)(uintptr_t)
   ((uint32_t)(uintptr_t)inherited_flags+count);
  if((int32_t)count>=0x800)count=0x800;
  if((int32_t)RESTORE_CRT_LOWIO_CAPACITY<(int32_t)count){
   block_index=1;
   do{
    block=FUN_00561b68(0x100);
    if(block==0){count=RESTORE_CRT_LOWIO_CAPACITY;break;}
    RESTORE_CRT_LOWIO_CAPACITY+=32u;
    RESTORE_CRT_LOWIO_BLOCKS[block_index]=block;
    initialize_lowio_block(block,&RESTORE_CRT_LOWIO_BLOCKS[block_index]);
    ++block_index;
   }while((int32_t)RESTORE_CRT_LOWIO_CAPACITY<(int32_t)count);
  }
  for(index=0;(int32_t)index<(int32_t)count;++index){
   uint32_t handle=inherited_handles[index];
   if(handle!=0xffffffffu){
    uint8_t flags=inherited_flags[index];
    if((flags&1u)!=0 && ((flags&8u)!=0 ||
       GetFileType((HANDLE)(uintptr_t)handle)!=0)){
     volatile RestoreCrtLowioRecord *record=
      RESTORE_CRT_LOWIO_BLOCKS[(int32_t)index>>5]+(index&31u);
     /* Native reloads the payload after GetFileType, using the fresh block
        table cell but the captured inherited-buffer base addresses. */
     record->handle=inherited_handles[index];
     record->flags=inherited_flags[index];
    }
   }
  }
 }
 for(index=0;index<3;++index){
  /* Keep this record across both Win32 callbacks; reload block0 next time. */
  volatile RestoreCrtLowioRecord *record=RESTORE_CRT_LOWIO_BLOCKS[0]+index;
  if(record->handle==0xffffffffu){
   uint32_t handle,type;
   record->flags=0x81;
   handle=(uint32_t)(uintptr_t)GetStdHandle(0xfffffff6u-index);
   if(handle!=0xffffffffu){
    type=GetFileType((HANDLE)(uintptr_t)handle);
    if(type!=0){
     type&=0xffu;
     record->handle=handle;
     if(type!=2){
      if(type==3)record->flags|=8u;
      continue;
     }
    }
   }
   record->flags|=0x40u;
  }else record->flags|=0x80u;
 }
 (void)SetHandleCount(RESTORE_CRT_LOWIO_CAPACITY);
}
void FUN_00566a03(void)
{restore_crt_initialize_lowio();}
