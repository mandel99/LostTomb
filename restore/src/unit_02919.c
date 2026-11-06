#include "crt_lowio.h"
extern void * __cdecl FUN_00561b68(size_t bytes);

/* Reuse the native scan and allocation loops. A returned descriptor is not
   marked open here; existing slots change only their handle DWORD. */
int32_t restore_crt_allocate_descriptor(void)
{
 int32_t selected=-1;
 uint32_t block_index;
 for(block_index=0;block_index<64;++block_index){
  RestoreCrtLowioRecord *volatile *slot=&RESTORE_CRT_LOWIO_BLOCKS[block_index];
  volatile RestoreCrtLowioRecord *record=*slot;
  uint32_t limit=(uint32_t)(uintptr_t)record+0x100u;
  if(record==0){
   record=FUN_00561b68(0x100);
   if(record!=0){
    RESTORE_CRT_LOWIO_CAPACITY+=32u;
    limit=(uint32_t)(uintptr_t)record+0x100u;
    *slot=(RestoreCrtLowioRecord *)record;
    while((uint32_t)(uintptr_t)record<limit){
     record->flags=0;
     record->handle=0xffffffffu;
     record->text_state=10;
     limit=(uint32_t)(uintptr_t)*slot+0x100u;
     record=(volatile RestoreCrtLowioRecord *)(uintptr_t)
      ((uint32_t)(uintptr_t)record+8u);
    }
    selected=(int32_t)(block_index<<5);
   }
   return selected;
  }
  /* Existing-block end is captured once, unlike new-block clear bounds. */
  while((uint32_t)(uintptr_t)record<limit){
   if((record->flags&1u)==0){
    uint32_t difference;
    record->handle=0xffffffffu;
    difference=(uint32_t)(uintptr_t)record-(uint32_t)(uintptr_t)*slot;
    selected=(int32_t)((uint32_t)((int32_t)difference>>3)+block_index*32u);
    if(selected!=-1)return selected;
    break;
   }
   record=(volatile RestoreCrtLowioRecord *)(uintptr_t)
    ((uint32_t)(uintptr_t)record+8u);
  }
 }
 return selected;
}
int32_t FUN_00566840(void)
{return restore_crt_allocate_descriptor();}
