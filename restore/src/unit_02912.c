#include "crt_lowio.h"
#include "crt_eh3_frames.h"
#include <windows.h>

/* Native EBP-12..EBP-1. Count and tail are ReadFile output cells, not zeroed
   locals. The three bytes before tail are never initialized or consumed. */
typedef struct RestoreCrtReadFrame {
 uint32_t transferred,total_or_end;
 uint8_t untouched[3],tail;
} RestoreCrtReadFrame;
typedef char RestoreCrtReadFrameLayout[(sizeof(RestoreCrtReadFrame)==12 &&
 offsetof(RestoreCrtReadFrame,transferred)==0 &&
 offsetof(RestoreCrtReadFrame,total_or_end)==4 &&
 offsetof(RestoreCrtReadFrame,tail)==11)?1:-1];
#define READ_RECORD(slot,offset) ((volatile RestoreCrtLowioRecord *)(uintptr_t) \
 ((uint32_t)(uintptr_t)*(slot)+(offset)))
#define READ_BYTE(address) (*(volatile uint8_t *)(uintptr_t)(address))
static void prepare_read(volatile RestoreCrtReadFrame *frame,
 const volatile uint32_t *arguments,uint32_t saved_sp)
{(void)frame;(void)arguments;(void)saved_sp;}

static int32_t run_read(volatile RestoreCrtReadFrame *frame,
 const volatile uint32_t *arguments,uint32_t parent_frame,uint32_t recovered)
{
 volatile uint32_t *a=(volatile uint32_t *)(uintptr_t)arguments;
 uint32_t descriptor=a[0],offset,output,destination;
 RestoreCrtLowioRecord *volatile *slot;
 volatile RestoreCrtLowioRecord *record;
 uint8_t flags;
 (void)parent_frame;(void)recovered;
 if(descriptor>=RESTORE_CRT_LOWIO_CAPACITY)goto invalid;
 offset=(descriptor&31u)*8u;
 slot=(RestoreCrtLowioRecord *volatile *)(uintptr_t)
  ((uint32_t)(uintptr_t)RESTORE_CRT_LOWIO_BLOCKS+
   (uint32_t)((int32_t)descriptor>>5)*4u);
 record=READ_RECORD(slot,offset);
 flags=record->flags;
 if((flags&1u)==0)goto invalid;
 frame->total_or_end=0;
 output=a[1];
 destination=output;
 if(a[2]==0 || (flags&2u)!=0)return 0;
 if((flags&0x48u)!=0){
  uint8_t pending=record->text_state;
  if(pending!=10){
   --a[2];
   READ_BYTE(output)=pending;
   destination=output+1u;
   frame->total_or_end=1;
   READ_RECORD(slot,offset)->text_state=10;
  }
 }
 if(!ReadFile((HANDLE)(uintptr_t)READ_RECORD(slot,offset)->handle,
   (void *)(uintptr_t)destination,a[2],(LPDWORD)&frame->transferred,0)){
  uint32_t error=GetLastError();
  if(error==5){RESTORE_CRT_ERRNO=9;RESTORE_CRT_DOSERRNO=5;return -1;}
  if(error==109)return 0;
  FUN_005672c8(error);
  return -1;
 }
 record=READ_RECORD(slot,offset);
 {
  uint32_t transferred=frame->transferred;
  frame->total_or_end+=transferred;
  flags=record->flags;
  if((flags&0x80u)==0)return (int32_t)frame->total_or_end;
  if(transferred!=0 && READ_BYTE(output)==10)flags|=4u;
  else flags&=0xfbu;
  record->flags=flags;
 }
 /* The native count argument becomes the text input cursor; keep its actual
    caller cell, while EDI/output retains the earlier buffer snapshot. */
 a[2]=a[1];
 frame->total_or_end+=a[1];
 for(;;){
  uint32_t end=frame->total_or_end;
  uint8_t character;
  if(a[2]>=end)break;
  character=READ_BYTE(a[2]);
  if(character==0x1a){
   record=READ_RECORD(slot,offset);
   flags=record->flags;
   if((flags&0x40u)==0)record->flags=flags|2u;
   break;
  }
  if(character!=13){
   READ_BYTE(output)=character;
   ++output;
   ++a[2];
  }else if(a[2]<end-1u){
   uint32_t next=a[2]+1u;
   if(READ_BYTE(next)==10){
    a[2]+=2u;
    READ_BYTE(output)=10;
    ++output;
   }else{
    READ_BYTE(output)=13;
    ++output;
    a[2]=next;
   }
  }else{
   BOOL ok;
   ++a[2];
   ok=ReadFile((HANDLE)(uintptr_t)READ_RECORD(slot,offset)->handle,
    (void *)&frame->tail,1,(LPDWORD)&frame->transferred,0);
   if((!ok && GetLastError()!=0) || frame->transferred==0){
    READ_BYTE(output)=13;
    ++output;
   }else if((READ_RECORD(slot,offset)->flags&0x48u)!=0){
    uint8_t tail=frame->tail;
    if(tail==10){READ_BYTE(output)=10;++output;}
    else{
     READ_BYTE(output)=13;
     ++output;
     READ_RECORD(slot,offset)->text_state=tail;
    }
   }else if(output==a[1] && frame->tail==10){
    READ_BYTE(output)=10;
    ++output;
   }else{
    (void)FUN_005662e3(a[0],-1,1);
    if(frame->tail!=10){READ_BYTE(output)=13;++output;}
   }
  }
 }
 frame->total_or_end=output-a[1];
 return (int32_t)frame->total_or_end;
invalid:
 RESTORE_CRT_DOSERRNO=0;
 RESTORE_CRT_ERRNO=9;
 return -1;
}
RESTORE_EH3_PARENT(restore_crt_read,int32_t,
 (uint32_t descriptor,uint8_t *buffer,uint32_t count),12,prepare_read,run_read)
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) int32_t FUN_005660ed(uint32_t descriptor,uint8_t *buffer,uint32_t count)
{__asm jmp restore_crt_read}
#pragma warning(pop)
