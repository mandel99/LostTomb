#ifndef RESTORE_CRT_EH3_FRAMES_H
#define RESTORE_CRT_EH3_FRAMES_H
#include "crt_eh3_handler.h"
#include "crt_exception_dispatch.h"
#include "crt_runtime_tail.h"

/* Native parent EBP is registration+16. Only actual locals are represented;
   the exception-pointers cell remains unwritten until dispatch supplies it. */
typedef struct RestoreCrtEh3Frame24 {
 uint32_t saved_sp,exception_pointers;
 RestoreCrtEh3Registration registration;
} RestoreCrtEh3Frame24;
typedef struct RestoreCrtEh3CleanupFrame {
 int32_t current_state;
 uint32_t saved_sp,exception_pointers;
 RestoreCrtEh3Registration registration;
} RestoreCrtEh3CleanupFrame;
typedef struct RestoreCrtEh3CatchFrame {
 uint32_t result,saved_exception_cell,skip_release,old_context,old_exception;
 uint32_t saved_sp,exception_pointers;
 RestoreCrtEh3Registration registration;
} RestoreCrtEh3CatchFrame;
typedef char RestoreCrtEh3FramesLayout[(sizeof(RestoreCrtEh3Frame24)==24 &&
 sizeof(RestoreCrtEh3CleanupFrame)==28 && sizeof(RestoreCrtEh3CatchFrame)==44 &&
 offsetof(RestoreCrtEh3Frame24,registration)==8 &&
 offsetof(RestoreCrtEh3CleanupFrame,registration)==12 &&
 offsetof(RestoreCrtEh3CatchFrame,registration)==28 &&
 offsetof(RestoreCrtEh3CatchFrame,saved_sp)==20)?1:-1];

void restore_crt_eh3_install(volatile RestoreCrtEh3Registration *,const RestoreCrtEh3Scope *);
void restore_crt_eh3_restore(const volatile RestoreCrtEh3Registration *);
__declspec(noreturn) void restore_crt_jump_to_catch(uint32_t destination,uint32_t frame);
__declspec(noreturn) void FUN_005614c3(uint32_t destination,uint32_t frame);
__declspec(noreturn) void restore_crt_invoke_terminate_handler(void);
void restore_crt_cleanup_to_state(uint32_t frame,uint32_t reserved,uint32_t descriptor,int32_t stop);
void FUN_00563be1(uint32_t frame,uint32_t reserved,uint32_t descriptor,int32_t stop);
uint32_t restore_crt_invoke_catch(uint32_t exception,uint32_t frame,uint32_t context,
 uint32_t descriptor,uint32_t handler,uint32_t depth,uint32_t code);
uint32_t FUN_00563d10(uint32_t exception,uint32_t frame,uint32_t context,
 uint32_t descriptor,uint32_t handler,uint32_t depth,uint32_t code);
int32_t restore_crt_cleanup_exception_filter(EXCEPTION_POINTERS *);
int32_t FUN_00563c7f(EXCEPTION_POINTERS *);
int32_t restore_crt_rethrow_filter(EXCEPTION_POINTERS *);
int32_t FUN_00563e1c(EXCEPTION_POINTERS *);

void restore_crt_copy_exception(uint32_t exception,uint32_t frame,uint32_t handler,uint32_t conversion);
void FUN_00563e46(uint32_t exception,uint32_t frame,uint32_t handler,uint32_t conversion);
void restore_crt_destroy_exception(int32_t exception,uint32_t mode);

/* Physical parent-frame boundaries shared by all three C-owned bodies.
   Prepare receives the real saved SP; body receives parent EBP and mode0.
   A recovery funclet uses that saved SP and completes the original parent. */
#define RESTORE_EH3_BODY_CALL(Bytes,Body,Mode) \
 __asm push Mode \
 __asm push ebp \
 __asm lea ecx,[ebp+8] \
 __asm push ecx \
 __asm lea eax,[ebp-Bytes] \
 __asm push eax \
 __asm call Body \
 __asm add esp,16
#define RESTORE_EH3_PARENT_RETURN \
 __asm pop edi \
 __asm pop esi \
 __asm pop ebx \
 __asm mov esp,ebp \
 __asm pop ebp \
 __asm ret
#define RESTORE_EH3_PARENT(Name,Result,Signature,Bytes,Prepare,Body) \
 __pragma(warning(push)) __pragma(warning(disable:4100)) \
 __declspec(naked) Result Name Signature { \
  __asm push ebp \
  __asm mov ebp,esp \
  __asm sub esp,Bytes \
  __asm push ebx \
  __asm push esi \
  __asm push edi \
  __asm mov edx,esp \
  __asm push edx \
  __asm lea ecx,[ebp+8] \
  __asm push ecx \
  __asm lea eax,[ebp-Bytes] \
  __asm push eax \
  __asm call Prepare \
  __asm add esp,12 \
  RESTORE_EH3_BODY_CALL(Bytes,Body,0) \
  RESTORE_EH3_PARENT_RETURN \
 } __pragma(warning(pop))
#define RESTORE_EH3_RECOVERY(Name,Bytes,Body) \
 __declspec(naked) uint32_t Name(void) { \
  __asm mov esp,[ebp-24] \
  RESTORE_EH3_BODY_CALL(Bytes,Body,1) \
  RESTORE_EH3_PARENT_RETURN \
 }
#endif
