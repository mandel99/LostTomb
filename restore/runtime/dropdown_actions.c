#include "dropdown_actions.h"
#include "dropdown_menu.h"

/* Preserve the native widths and store order. Tracing is test-only. */
#ifdef RESTORE_DROPDOWN_ACTION_TRACE
extern void restore_dropdown_action_write(uint32_t address, uint32_t width, uint32_t value);
#define PUT32(a,v) restore_dropdown_action_write((a),4,(v))
#define PUT8(a,v) restore_dropdown_action_write((a),1,(v))
#define PUT16(a,v) restore_dropdown_action_write((a),2,(v))
#else
#define PUT32(a,v) (DM_U(a)=(v))
#define PUT8(a,v) (DM_B(a)=(uint8_t)(v))
#define PUT16(a,v) (DM_W(a)=(uint16_t)(v))
#endif

void FUN_00402bc0(void)
{
    PUT8(0xe38dda,10); PUT32(0xf53f18,0); PUT8(0xe38dca,1);
}
void FUN_00402be0(void)
{
    uint32_t value=DM_U(0xe38ef0);
    PUT8(0xe38dda,0x56); PUT32(0x6de510,value);
    PUT32(0xf53f4c,0); PUT8(0xe38dca,1);
}
void FUN_00402e30(void)
{
    PUT8(0xe38dda,0x45); PUT32(0xf53f18,0); PUT8(0xe38dca,1);
}
void FUN_00402ee0(void)
{
    uint32_t packed=DM_U(0xe38e64), value=DM_U(0xe38ec8), next;
    PUT32(0x6057bc,value);
    value=DM_U(0xe38ecc);
    PUT32(0x6057c0,packed&255u);
    next=DM_B(0xe38e63);
    PUT32(0x6057b4,value);
    value=DM_U(0xe38ec0);
    PUT32(0x6057b8,(packed>>8)&255u);
    packed=DM_U(0xe38ec4);
    PUT32(0x6057b0,next);
    next=DM_U(0xe38ed0);
    PUT32(0x6057ac,packed); PUT32(0x6057a8,value&255u);
    PUT32(0x6057a4,next); PUT8(0xe38dda,11);
    PUT32(0xf53f1c,0); PUT8(0xe38dca,1);
}
void FUN_004032c0(void)
{
    uint32_t speed=DM_U(0xe38e6c), scroll=DM_U(0xe38e70);
    PUT32(0x6057a0,speed); PUT32(0x60579c,scroll);
    PUT8(0xe38dda,12); PUT32(0xf53f20,0); PUT8(0xe38dca,1);
}
void FUN_00406070(void)
{
    uint32_t address;
    for(address=0x784cd0;address<0x784ce0;address+=4){
        PUT32(address-16,UINT32_MAX); PUT32(address,UINT32_MAX);
    }
}
void FUN_004060e0(void)
{
    PUT32(0x784b64,UINT32_MAX); PUT32(0x784b6c,UINT32_MAX);
    PUT32(0x784b68,UINT32_MAX); PUT32(0x784b70,UINT32_MAX);
}
void FUN_00406100(void)
{
    PUT32(0x784b74,UINT32_MAX); PUT32(0x784b84,UINT32_MAX);
    PUT32(0x784b78,UINT32_MAX); PUT32(0x784b88,UINT32_MAX);
    PUT32(0x784b7c,UINT32_MAX); PUT32(0x784b8c,UINT32_MAX);
    PUT32(0x784b80,UINT32_MAX); PUT32(0x784b90,UINT32_MAX);
}
void FUN_00406140(void)
{
    PUT32(0x784dc4,UINT32_MAX); PUT32(0x784dd0,UINT32_MAX);
    PUT32(0x784dc8,UINT32_MAX); PUT32(0x784dd4,UINT32_MAX);
    PUT32(0x784dcc,UINT32_MAX); PUT32(0x784dd8,UINT32_MAX);
}
void FUN_00406170(void)
{
    uint32_t address;
    for(address=0x784be0;address<0x784c00;address+=2){
        PUT16(address-32,65535); PUT16(address,65535);
    }
}
int restore_dropdown_action(uint32_t token)
{
    switch(token){
    case 0x402bc0:FUN_00402bc0();break;
    case 0x402be0:FUN_00402be0();break;
    case 0x402e30:FUN_00402e30();break;
    case 0x402ee0:FUN_00402ee0();break;
    case 0x4032c0:FUN_004032c0();break;
    case 0x406070:FUN_00406070();break;
    case 0x4060e0:FUN_004060e0();break;
    case 0x406100:FUN_00406100();break;
    case 0x406140:FUN_00406140();break;
    case 0x406170:FUN_00406170();break;
    default:return 0;
    }
    return 1;
}
