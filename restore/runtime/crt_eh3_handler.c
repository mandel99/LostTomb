#include "crt_eh3_handler.h"

/* Unlike restore_crt_run_scope, this bridge also returns the callback's real
   EAX. A zero filter result or a returning handler becomes the next filter's
   EAX input. ECX is recomputed as three times the current scope index.
   Callback EDX and flags are not C inputs: the relocated scope consumers use
   their parent frame; arbitrary foreign funclets need separate qualification. */
#pragma warning(push)
#pragma warning(disable:4100)
static __declspec(naked) uint32_t invoke_scope(uint32_t target,
 uint32_t parent_frame,uint32_t entry_eax,uint32_t entry_ecx)
{
 __asm {
  push ebp
  push ebx
  push esi
  push edi
  mov ebp,[esp+24]
  mov eax,[esp+28]
  mov ecx,[esp+32]
  call dword ptr [esp+20]
  pop edi
  pop esi
  pop ebx
  pop ebp
  ret
 }
}
#pragma warning(pop)

static const volatile RestoreCrtEh3Scope *scope_at(
 const RestoreCrtEh3Scope *table,int32_t state)
{
 return (const volatile RestoreCrtEh3Scope *)(uintptr_t)
  ((uint32_t)(uintptr_t)table+(uint32_t)state*12u);
}

uint32_t __cdecl restore_crt_eh3_handler(EXCEPTION_RECORD *exception,
 RestoreCrtEh3Registration *registration,void *context,void *dispatcher)
{
 EXCEPTION_POINTERS pointers;
 RestoreCrtEh3Registration *volatile *registration_slot=&registration;
 const RestoreCrtEh3Scope *table;
 int32_t state;
 uint32_t callback_eax;
 (void)dispatcher;
 __asm cld

 /* 561A9B..561AA8: unwind does not replace the exception-pointers cell. */
 if((exception->ExceptionFlags&6u)!=0){
  restore_crt_local_unwind((uint32_t)(uintptr_t)registration,-1,
   (uint32_t)(uintptr_t)registration+16u);
  return 1;
 }

 /* 561AAE..561AC0: this cell is valid while dispatch is active, just as the
    native pointer refers into the dispatcher's temporary stack frame. */
 pointers.ExceptionRecord=exception;
 pointers.ContextRecord=(PCONTEXT)context;
 callback_eax=(uint32_t)(uintptr_t)&pointers;
 *(EXCEPTION_POINTERS *volatile *)(uintptr_t)
  ((uint32_t)(uintptr_t)registration-4u)=&pointers;
 state=registration->state;
 table=registration->scopes;

 for(;;){
  const volatile RestoreCrtEh3Scope *scope;
  uint32_t index3;
  RestoreCrtEh3Filter filter;
  if(state==-1)return 1; /* 561B29 */
  index3=(uint32_t)state*3u;
  scope=scope_at(table,state);
  filter=scope->filter;
  if(filter!=0){
   callback_eax=invoke_scope((uint32_t)(uintptr_t)filter,
    (uint32_t)(uintptr_t)registration+16u,callback_eax,index3);
   /* 561ADD reloads [dispatch EBP+12] even when a returning selected
      handler has left the dispatcher's logical EBP at the parent frame. */
   registration=*registration_slot;
   if((int32_t)callback_eax<0)return 0; /* 561B22 */
   if(callback_eax!=0){
    uint32_t parent_frame=(uint32_t)(uintptr_t)registration+16u;
    RestoreCrtEh3Handler target;
    int32_t enclosing;
    /* 561AE6 fixes EDI before either unwind helper can change the table. */
    table=registration->scopes;
    restore_crt_global_unwind(registration);
    /* 561AF2 deliberately changes the dispatch EBP permanently. Selected
       native handlers restore the parent's ESP and jump to a continuation.
       If one instead returns, retain the real subsequent loop/reload rule. */
    registration_slot=(RestoreCrtEh3Registration *volatile *)(uintptr_t)
     (parent_frame+12u);
    restore_crt_local_unwind((uint32_t)(uintptr_t)registration,state,parent_frame);
    scope=scope_at(table,state);
    target=scope->handler;
    /* Notify1 reads [parent EBP+8]; the pushed literal one is not its code. */
    restore_crt_notify_frame((uint32_t)(uintptr_t)target,parent_frame);
    enclosing=scope->enclosing;
    registration->state=enclosing;
    target=scope->handler;
    callback_eax=invoke_scope((uint32_t)(uintptr_t)target,parent_frame,
     (uint32_t)enclosing,index3);
   }
  }
  /* 561B17..561B20 reload the table, then the enclosing index, even after
     a selected handler unexpectedly returns. EAX remains the callback result. */
  table=registration->scopes;
  state=scope_at(table,state)->enclosing;
 }
}

uint32_t __cdecl FUN_00561a90(EXCEPTION_RECORD *exception,
 RestoreCrtEh3Registration *registration,void *context,void *dispatcher)
{
 return restore_crt_eh3_handler(exception,registration,context,dispatcher);
}
