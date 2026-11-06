#include "crt_lowio.h"
#include "crt_eh3_frames.h"
#include <windows.h>

/* Native EBP-414h..EBP-1. Buffer padding, written count and binary-mode
   source cursor remain actual stack bytes until the native path writes them. */
typedef struct RestoreCrtWriteFrame {
 uint8_t buffer[1028];
 uint32_t inserted_cr,written,total,cursor;
} RestoreCrtWriteFrame;
typedef char RestoreCrtWriteFrameLayout[(sizeof(RestoreCrtWriteFrame)==1044 &&
 offsetof(RestoreCrtWriteFrame,inserted_cr)==1028 &&
 offsetof(RestoreCrtWriteFrame,written)==1032 &&
 offsetof(RestoreCrtWriteFrame,total)==1036 &&
 offsetof(RestoreCrtWriteFrame,cursor)==1040)?1:-1];
#define WRITE_RECORD(slot,offset) ((volatile RestoreCrtLowioRecord *)(uintptr_t) \
 ((uint32_t)(uintptr_t)*(slot)+(offset)))
#define WRITE_BYTE(address) (*(volatile uint8_t *)(uintptr_t)(address))
static void prepare_write(volatile RestoreCrtWriteFrame *frame,
 const volatile uint32_t *arguments,uint32_t saved_sp)
{(void)frame;(void)arguments;(void)saved_sp;}

static int32_t run_write(volatile RestoreCrtWriteFrame *frame,
 const volatile uint32_t *arguments,uint32_t parent_frame,uint32_t recovered)
{
 volatile uint32_t *a=(volatile uint32_t *)(uintptr_t)arguments;
 uint32_t descriptor=a[0],offset,zero_request;
 RestoreCrtLowioRecord *volatile *slot;
 volatile RestoreCrtLowioRecord *record;
 uint8_t flags;
 (void)parent_frame;(void)recovered;
 if(descriptor>=RESTORE_CRT_LOWIO_CAPACITY)goto invalid;
 offset=(descriptor&31u)*8u;
 slot=(RestoreCrtLowioRecord *volatile *)(uintptr_t)
  ((uint32_t)(uintptr_t)RESTORE_CRT_LOWIO_BLOCKS+
   (uint32_t)((int32_t)descriptor>>5)*4u);
 flags=WRITE_RECORD(slot,offset)->flags;
 if((flags&1u)==0)goto invalid;
 zero_request=a[2]==0;
 frame->total=0;
 frame->inserted_cr=0;
 if(zero_request)return 0;
 if((flags&0x20u)!=0)(void)FUN_005662e3(descriptor,0,2);
 record=WRITE_RECORD(slot,offset);
 if((record->flags&0x80u)==0){
  if(!WriteFile((HANDLE)(uintptr_t)record->handle,(const void *)(uintptr_t)a[1],
      a[2],(LPDWORD)&frame->written,0))a[0]=GetLastError();
  else{
   uint32_t written=frame->written;
   a[0]=0;
   frame->total=written;
  }
 }else{
  uint32_t buffer=(uint32_t)(uintptr_t)frame->buffer;
  frame->cursor=a[1];
  a[0]=0;
  if(a[2]!=0){
   for(;;){
    uint32_t output=buffer,length,written;
    do{
     uint32_t source;
     uint8_t character;
     if(a[2]<=frame->cursor-a[1])break;
     source=frame->cursor;
     ++frame->cursor; /* Native advances the cell before fetching its BYTE. */
     character=WRITE_BYTE(source);
     if(character==10){
      ++frame->inserted_cr;
      WRITE_BYTE(output)=13;
      ++output;
     }
     WRITE_BYTE(output)=character;
     ++output;
     /* Check AFTER the complete character/CRLF pair: length may be1025. */
    }while((int32_t)(output-buffer)<1024);
    length=output-buffer;
    if(!WriteFile((HANDLE)(uintptr_t)WRITE_RECORD(slot,offset)->handle,
        (const void *)(uintptr_t)buffer,length,(LPDWORD)&frame->written,0)){
     a[0]=GetLastError();
     break;
    }
    written=frame->written;
    frame->total+=written;
    if((int32_t)written<(int32_t)length || a[2]<=frame->cursor-a[1])break;
   }
  }
 }
 {
  uint32_t total=frame->total;
  /* All packed LF insertions participate, including a failed/partial batch. */
  if(total!=0)return (int32_t)(total-frame->inserted_cr);
 }
 if(a[0]!=0){
  if(a[0]==5){RESTORE_CRT_ERRNO=9;RESTORE_CRT_DOSERRNO=5;return -1;}
  FUN_005672c8(a[0]);
  return -1;
 }
 if((WRITE_RECORD(slot,offset)->flags&0x40u)!=0 && WRITE_BYTE(a[1])==0x1a)return 0;
 RESTORE_CRT_ERRNO=28;
 RESTORE_CRT_DOSERRNO=0;
 return -1;
invalid:
 RESTORE_CRT_DOSERRNO=0;
 RESTORE_CRT_ERRNO=9;
 return -1;
}
RESTORE_EH3_PARENT(restore_crt_write,int32_t,
 (uint32_t descriptor,const uint8_t *buffer,uint32_t count),1044,prepare_write,run_write)
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) int32_t FUN_005664d5(uint32_t descriptor,const uint8_t *buffer,uint32_t count)
{__asm jmp restore_crt_write}
#pragma warning(pop)
