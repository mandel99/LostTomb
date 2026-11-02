#include "restore_types.h"
#include "mission_compat.h"
#include "actor_teardown_model.h"
#define H_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
void FUN_004f6810(RestoreDynamicHistory *history,RestoreHistorySnapshot *snapshot)
{
    uint32_t self=(uint32_t)(uintptr_t)history,out=(uint32_t)(uintptr_t)snapshot;
    uint32_t source,words,i;
    if((int32_t)H_U(self+16u)>200)H_U(self+16u)=200;
    (void)restore_ring_004f6a10(history,(volatile uint32_t *)(uintptr_t)(self+4u));
    (void)restore_ring_004f6a10(history,(volatile uint32_t *)(uintptr_t)(self+8u));
    (void)restore_ring_004f6a10(history,(volatile uint32_t *)(uintptr_t)(self+12u));
    /* Native stores are ordered even when output overlaps the object. */
    H_U(out)=H_U(self+12u);
    H_U(out+4u)=H_U(self+16u);
    H_U(out+8u)=H_U(self+4u);
    H_U(out+12u)=H_U(self+8u);
    H_B(out+16u)=H_B(self);
    source=H_U(self+20u);
    if(source) {
        words=(H_U(self+16u)*4u)>>2;
        /* Forward REP MOVSD order, including overlapping value buffers. */
        for(i=0;i<words;++i)H_U(out+20u+i*4u)=H_U(source+i*4u);
    } else {
        for(i=0;i<200u;++i)H_U(out+20u+i*4u)=0;
    }
}
