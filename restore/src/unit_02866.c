#include "crt_eh3_frames.h"
#include "legacy_member_calls.h"

uint32_t *FUN_00562880(void *,const void *,uint32_t);
int32_t FUN_00564071(int32_t,int32_t *);
uint8_t FUN_0056878b(void *,uintptr_t);
uint8_t FUN_005687a7(FARPROC);
__declspec(noreturn) void FUN_005637ee(void);
uint32_t restore_crt_copy_exception_recover(void);
static int32_t restore_crt_copy_exception_filter(void){return 1;}
const RestoreCrtEh3Scope restore_crt_copy_exception_scopes[1]={
 {-1,restore_crt_copy_exception_filter,restore_crt_copy_exception_recover}
};
/* The native extra DWORD at EBP-28 is not read by this routine or its funclets. */
typedef struct RestoreCrtCopyExceptionFrame {
 uint32_t reserved;
 RestoreCrtEh3Frame24 eh3;
} RestoreCrtCopyExceptionFrame;
typedef char RestoreCrtCopyExceptionLayout[(sizeof(RestoreCrtCopyExceptionFrame)==28 &&
 offsetof(RestoreCrtCopyExceptionFrame,eh3)==4)?1:-1];
static void prepare_copy_exception(volatile RestoreCrtCopyExceptionFrame *f,
 const volatile uint32_t *arguments,uint32_t saved_sp)
{
 (void)arguments;
 restore_crt_eh3_install(&f->eh3.registration,restore_crt_copy_exception_scopes);
 f->eh3.saved_sp=saved_sp;
}
#define READ32(address) restore_crt_eh_read32((uint32_t)(uintptr_t)(address))
#define READ8(address) (*(volatile const uint8_t *)(uintptr_t)(address))
static uint32_t adjusted_object(uint32_t object,uint32_t conversion)
{
 return (uint32_t)FUN_00564071((int32_t)object,(int32_t *)(uintptr_t)(conversion+8));
}
static void run_copy_exception(volatile RestoreCrtCopyExceptionFrame *f,
 const volatile uint32_t *arguments,uint32_t parent_frame,uint32_t recovered)
{
 uint32_t handler=arguments[2],type=READ32(handler+4),destination;
 uint32_t exception,conversion,object;
 (void)parent_frame;(void)recovered;
 if(type==0 || READ8(type+8)==0 || READ32(handler+8)==0)goto done;
 destination=READ32(handler+8)+arguments[1]+12;
 f->eh3.registration.state=0;
 if(READ8(handler)&8){
  exception=arguments[0];
  if(!FUN_0056876f((void *)(uintptr_t)READ32(exception+24),1) ||
     !FUN_0056878b((void *)(uintptr_t)destination,1))goto invalid;
  object=READ32(exception+24);
  *(uint32_t *)(uintptr_t)destination=object;
  conversion=arguments[3];
  *(uint32_t *)(uintptr_t)destination=adjusted_object(object,conversion);
 }else{
  conversion=arguments[3];
  if(READ8(conversion)&1){
   exception=arguments[0];
   if(!FUN_0056876f((void *)(uintptr_t)READ32(exception+24),1) ||
      !FUN_0056878b((void *)(uintptr_t)destination,1))goto invalid;
   FUN_00562880((void *)(uintptr_t)destination,(const void *)(uintptr_t)READ32(exception+24),READ32(conversion+20));
   if(READ32(conversion+20)==4 && (object=READ32(destination))!=0)
    *(uint32_t *)(uintptr_t)destination=adjusted_object(object,conversion);
  }else{
   /* Native captures this branch before the validation callbacks. */
   uint32_t has_constructor=READ32(conversion+24)!=0;
   exception=arguments[0];
   if(!FUN_0056876f((void *)(uintptr_t)READ32(exception+24),1) ||
      !FUN_0056878b((void *)(uintptr_t)destination,1))goto invalid;
   if(!has_constructor){
    /* Size is pushed before PMD adjustment, and survives that call. */
    uint32_t size=READ32(conversion+20);
    object=adjusted_object(READ32(exception+24),conversion);
    FUN_00562880((void *)(uintptr_t)destination,(const void *)(uintptr_t)object,size);
   }else{
    if(!FUN_005687a7((FARPROC)(uintptr_t)READ32(conversion+24)))goto invalid;
    if(READ8(conversion)&4){
     object=adjusted_object(READ32(exception+24),conversion);
     restore_call_member2((void *)(uintptr_t)destination,READ32(conversion+24),object,1);
    }else{
     object=adjusted_object(READ32(exception+24),conversion);
     restore_call_member1((void *)(uintptr_t)destination,READ32(conversion+24),object);
    }
   }
  }
 }
 f->eh3.registration.state=-1;
 done:restore_crt_eh3_restore(&f->eh3.registration);return;
 invalid:FUN_00563844();
}
RESTORE_EH3_PARENT(restore_crt_copy_exception,void,
 (uint32_t exception,uint32_t frame,uint32_t handler,uint32_t conversion),28,
 prepare_copy_exception,run_copy_exception)
/* A failed catch construction terminates after restoring the native parent SP. */
__declspec(naked) uint32_t restore_crt_copy_exception_recover(void)
{__asm mov esp,[ebp-24] __asm jmp FUN_005637ee}
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) void FUN_00563e46(uint32_t exception,uint32_t frame,uint32_t handler,uint32_t conversion)
{__asm jmp restore_crt_copy_exception}

#pragma warning(pop)
