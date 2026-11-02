#include "editor_ring.h"
/* Real C dependency; its editor algorithm still needs native verification. */
extern uint32_t FUN_004ecec0(int32_t linear,int32_t tool,uint32_t *failure_flags);
/* Explicit callback entry from original 004ed330, reconstructed from pinned PE CFG.
   It is absent from the current Ghidra function-object cache. No address-token call. */
uint8_t __cdecl FUN_004ed2d0(uint32_t linear,int32_t x,int32_t y,uint32_t context)
{
    uint32_t tool=ERR32(0x5e6970u),original=linear,delta;
    /* Native ESI preserves the cell before its argument slot becomes a failure mask. */
    (void)x;(void)y;(void)context;
    if((uint8_t)FUN_004ecec0((int32_t)original,(int32_t)tool,&linear)){
        delta=original-ERR32(0xbca2d8u);
        ERW32(0xbca300u,original);
        ERW32(0xbca304u,(int32_t)delta%228);
        ERW32(0xbca308u,(int32_t)delta/228);
        return 0;
    }
    return 1;
}
void FUN_004ed330(void)
{
    uint32_t tool=ERR32(0x5e6970u),linear=ERR32(0xbca2c4u),radius;
    radius=ERR32(0x5f5d1cu+tool*4u);
    FUN_004ed0d0(linear,(int32_t)radius,FUN_004ed2d0,0);
}
