#include "crt_multibyte.h"
#include "crt_eh3_frames.h"
#include "crt_nls_mapping.h"

/* Native locals occupy EBP-514h..EBP-1. The three output buffers are not
   initialized before unchecked child calls; retain the actual stack bytes. */
typedef struct RestoreCrtMultibyteCaseFrame {
 uint16_t attributes[256];
 uint8_t upper[256],lower[256],input[256];
 CPINFO info;
} RestoreCrtMultibyteCaseFrame;
typedef char RestoreCrtMultibyteCaseFrameLayout[(sizeof(RestoreCrtMultibyteCaseFrame)==1300 &&
 offsetof(RestoreCrtMultibyteCaseFrame,upper)==512 &&
 offsetof(RestoreCrtMultibyteCaseFrame,lower)==768 &&
 offsetof(RestoreCrtMultibyteCaseFrame,input)==1024 &&
 offsetof(RestoreCrtMultibyteCaseFrame,info)==1280)?1:-1];
static void prepare_case_tables(volatile RestoreCrtMultibyteCaseFrame *f,
 const volatile uint32_t *arguments,uint32_t saved_sp)
{(void)f;(void)arguments;(void)saved_sp;}
static void run_case_tables(volatile RestoreCrtMultibyteCaseFrame *f,
 const volatile uint32_t *arguments,uint32_t parent_frame,uint32_t recovered)
{
 uint32_t i;
 (void)arguments;(void)parent_frame;(void)recovered;
 if(GetCPInfo(RESTORE_CRT_CODEPAGE,(LPCPINFO)&f->info)==1){
  for(i=0;i<256;++i)f->input[i]=(uint8_t)i;
  f->input[0]=0x20;
  /* Native skips each lead-byte interval by replacing its input bytes. */
  for(i=0;f->info.LeadByte[i]!=0;i+=2){
   uint32_t first=f->info.LeadByte[i],last=f->info.LeadByte[i+1];
   while(first<=last)f->input[first++]=0x20;
  }
  (void)FUN_00569335(1,(LPCSTR)f->input,256,(LPWORD)f->attributes,
   RESTORE_CRT_CODEPAGE,RESTORE_CRT_MB_LOCALE,0);
  (void)FUN_0056947e(RESTORE_CRT_MB_LOCALE,0x100,(LPCSTR)f->input,256,
   (LPSTR)f->lower,256,RESTORE_CRT_CODEPAGE,0);
  (void)FUN_0056947e(RESTORE_CRT_MB_LOCALE,0x200,(LPCSTR)f->input,256,
   (LPSTR)f->upper,256,RESTORE_CRT_CODEPAGE,0);
  for(i=0;i<256;++i){
   uint16_t attributes=f->attributes[i];
   if(attributes&1){RESTORE_CRT_MB_FLAGS[i+1]|=0x10;RESTORE_CRT_MB_CASE_MAP[i]=f->lower[i];}
   else if(attributes&2){RESTORE_CRT_MB_FLAGS[i+1]|=0x20;RESTORE_CRT_MB_CASE_MAP[i]=f->upper[i];}
   else RESTORE_CRT_MB_CASE_MAP[i]=0;
  }
 }else{
  for(i=0;i<256;++i){
   if(i>=0x41 && i<=0x5a){RESTORE_CRT_MB_FLAGS[i+1]|=0x10;RESTORE_CRT_MB_CASE_MAP[i]=(uint8_t)(i+0x20);}
   else if(i>=0x61 && i<=0x7a){RESTORE_CRT_MB_FLAGS[i+1]|=0x20;RESTORE_CRT_MB_CASE_MAP[i]=(uint8_t)(i-0x20);}
   else RESTORE_CRT_MB_CASE_MAP[i]=0;
  }
 }
}
/* Reuse the existing physical parent-frame entry macro without installing EH3.
   Only stack ownership is assembly; all table/branch logic is C. */
RESTORE_EH3_PARENT(restore_crt_build_multibyte_case_tables,void,(void),1300,
 prepare_case_tables,run_case_tables)
__declspec(naked) void FUN_00569a70(void)
{__asm jmp restore_crt_build_multibyte_case_tables}
