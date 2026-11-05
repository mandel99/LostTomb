#include "native_member_bridge.h"
#include <stdlib.h>

void restore_native_member_run(uint32_t target,struct RestoreNativeMemberFrame *f,
                               uint32_t cleanup,RestoreNativeRaw body)
{
    uint32_t r[8],flags=f->flags,native_sp=f->saved_esp+4u,control_sp;
    __asm { mov control_sp,esp }
    /* Reject misuse of this private snapshot API before its modeled PUSHes
       could overwrite the C control frame. Canonical entries reserve it. */
    if(native_sp<control_sp||native_sp-control_sp<0x10000u)abort();
    r[0]=f->eax;r[1]=f->ecx;r[2]=f->edx;r[3]=f->ebx;
    r[4]=native_sp;r[5]=f->ebp;r[6]=f->esi;r[7]=f->edi;
    body(target,r,&flags);
    if(r[4]!=native_sp+4u+cleanup)abort();
    f->eax=r[0];f->ecx=r[1];f->edx=r[2];f->ebx=r[3];
    f->ebp=r[5];f->esi=r[6];f->edi=r[7];f->flags=flags;
    /* saved_esp remains the true physical return-stack anchor. POPAD ignores
       its ESP slot; the entry restores it without changing modeled flags. */
}
