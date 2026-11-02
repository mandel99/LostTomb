#include "construction_confirm_actions.h"
#include "construction_controller.h"

/* Pinned PE SHA256 fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d.
   Whole19-instruction bodies4021D0..402208,402210..402248,
   402250..402288 and402290..4022C8. Alignment NOPs are excluded.
   Each has one direct CALL474700, five DWORD arguments, caller cleanup20B.
   DEC AL;NEG AL;SBB EAX,EAX;AND AL,FE;INC EAX maps answer1 to1,
   and all other BYTE values toFFFFFFFF, independent of incoming EAX. */
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

static void confirm(uint32_t decision)
{
    uint8_t answer=B(0xd3b439u);
    uint32_t y2=U(0xbc9698u);
    uint32_t x2=U(0xbc969cu);
    uint32_t x1=U(0xbc96a4u);
    uint32_t y1;
    /* Native reads y1 only after storing the selected decision field. */
    U(decision)=answer==1u?1u:UINT32_MAX;
    y1=U(0xbc96a0u);
    FUN_00474700(0,(int32_t)x1,(int32_t)y1,(int32_t)x2,(int32_t)y2);
}

void restore_construction_confirm_004021d0(void){confirm(0xbc96acu);}
void restore_construction_confirm_00402210(void){confirm(0xbc96a8u);}
void restore_construction_confirm_00402250(void){confirm(0xbc96b4u);}
void restore_construction_confirm_00402290(void){confirm(0xbc96b0u);}

int restore_construction_confirm_action(uint32_t token)
{
    switch(token){
    case 0x4021d0u:restore_construction_confirm_004021d0();return 1;
    case 0x402210u:restore_construction_confirm_00402210();return 1;
    case 0x402250u:restore_construction_confirm_00402250();return 1;
    case 0x402290u:restore_construction_confirm_00402290();return 1;
    default:return 0;
    }
}
