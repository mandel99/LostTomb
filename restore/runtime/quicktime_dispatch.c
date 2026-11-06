#include "quicktime_dispatch.h"
#include <string.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(a))
/* Native560350 writes only AX; keep the selector's actual upper16 bits. */
__declspec(naked) uint32_t restore_quicktime_missing_dispatcher(void)
{
    __asm {
        mov ax,0f7d1h
        ret
    }
}
static uint32_t code_address(uint32_t (__cdecl *function)(void))
{
    uint32_t result;
    typedef char QuickTimeCallbackMustFitDWORD[(sizeof(function)==sizeof(result))?1:-1];
    memcpy(&result,&function,sizeof(result));
    return result;
}
/* These are code-address relocations, not alternate native state values.
   Other slots, including genuine null GetProcAddress results, stay unchanged. */
void restore_quicktime_relocate_slots(void)
{
    uint32_t error=code_address(restore_quicktime_missing_dispatcher);
    if(U(0x604804u)==0x560360u){
        void (__cdecl *initializer)(void)=FUN_00560360;
        uint32_t target;
        memcpy(&target,&initializer,sizeof(target));
        U(0x604804u)=target;
    }
    /* This layer invokes only the selector dispatcher. The other two slots
       are GetProcAddress outputs and keep their original tokens until written. */
    if(U(0x604808u)==0x560350u)U(0x604808u)=error;
}
