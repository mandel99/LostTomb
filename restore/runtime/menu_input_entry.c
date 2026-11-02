#include "menu_input_entry.h"
#include "menu_input_boundary.h"
#include "actor_panel_native_context.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error This native entry requires MSVC Win32 PUSHFD/PUSHAD semantics.
#endif
typedef char entry_pointer_is32[sizeof(void*)==4?1:-1];
typedef char entry_frame_layout[offsetof(struct restore_menu_input_frame,flags)==32?1:-1];
static __declspec(noreturn) void entry_failure(const char *reason)
{fprintf(stderr,"4D8310 entry: %s\n",reason);abort();}
static __declspec(noinline) void __cdecl entry_invoke(const uint32_t *saved)
{
 uint32_t stack[4096],i,entry;struct restore_menu_input_frame f;
 RestoreActorPanelNativeScope previous_context;
 /* PUSHFD then PUSHAD: EDI,ESI,EBP,savedESP,EBX,EDX,ECX,EAX,flags,RA.
    The captured savedESP is the value immediately after PUSHFD. */
 if(restore_menu_input_boundary_closed_103!=RESTORE_MENU_INPUT_CLOSED_TAG)entry_failure("all103 genuine targets are not closed");
 if(saved[3]+4u!=(uint32_t)(uintptr_t)(saved+9))entry_failure("snapshot layout");
 for(i=0;i<4096;++i)stack[i]=UINT32_C(0xa5a55a5a);
 entry=(uint32_t)(uintptr_t)(stack+4000);stack[4000]=saved[9];
 f.r[0]=saved[7];f.r[1]=saved[6];f.r[2]=saved[5];f.r[3]=saved[4];
 f.r[4]=entry;f.r[5]=saved[2];f.r[6]=saved[1];f.r[7]=saved[0];
 f.flags=saved[8];f.boundary=restore_menu_input_boundary_dispatch;
 /* Native own-frame minimum is entry-196 including CALL return slots;
    all131 RET paths balance to entry. Reserve16KiB for composition, versus
    the accepted8KiB4F7E50 raw-frame budget. Indexed local flags occupy the
    last52 native local bytes; genuine record counts fit their50-point data.
    Nested typed facades allocate their own frames. Never use caller stack
    as scratch, and never invoke an original code address from return data. */
 previous_context=restore_actor_panel_native_enter();
 restore_menu_input_controller(&f);
 restore_actor_panel_native_leave(previous_context);
 if(f.r[4]!=entry+4u||stack[4000]!=saved[9])entry_failure("return slot or RET0 balance");
 if(f.r[3]!=saved[4]||f.r[5]!=saved[2]||f.r[6]!=saved[1]||f.r[7]!=saved[0])entry_failure("model nonvolatile registers");
 for(i=0;i<16;++i)if(stack[i]!=UINT32_C(0xa5a55a5a))entry_failure("lower emulated-stack guard");
 for(i=4001;i<4096;++i)if(stack[i]!=UINT32_C(0xa5a55a5a))entry_failure("upper emulated-stack guard");
}
/* Snapshot before any compiler prologue can assign GPRs. No x87 operations
   or CW changes are introduced. CLD supplies the Win32 C calling convention
   during C execution; the captured flags and all GPRs are restored on return.
   Known callers4D6C67/538CA3 consume none of the volatile return registers. */
__declspec(naked) void __cdecl FUN_004d8310(void)
{
 __asm {
  pushfd
  pushad
  mov eax,esp
  cld
  push eax
  call entry_invoke
  add esp,4
  popad
  popfd
  ret
 }
}
