#include "actor_update.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define H(a) (*(int16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
extern void FUN_00489a50(uint32_t);
extern void FUN_00489a80(void);
extern void FUN_004b4980(uint32_t);
void FUN_00489b70(void)
{
    uint32_t player=U(0xeaac28)*0x49d8u;
    U(0xea5ea4+player)=0;U(0xea5c9c+player)=0;U(0xea5f98+player)=0;
    H(0xea5cea+player)=0;U(0xea5eb4+player)=0;U(0xea5ee8+player)=0;U(0xea5f74+player)=0;
    U(0xd056f4)=0;
    if(I(0xea5db4+player)>0)U(0xea5db4+player)--;
    U(0xd056f8)=1;
    do {
        uint32_t id=U(0xd056f8),offset=id*388u,actor=0xbcd600u+offset;
        if(B(actor+6)) {
            int32_t partner=H(actor+0x7c);
            if(partner) {
                uint32_t other=0xbcd600u+(uint32_t)partner*388u;
                if(B(other+6)!=1)H(actor+0x7c)=0;
                if(H(other+0x7a)!=(int32_t)id)H(actor+0x7c)=0;
            }
            U(0xd056f4)++;
            FUN_00489a50(B(actor+2));
            id=U(0xd056f8);
            restore_actor_callback(U(0x5dc28cu+(uint32_t)B(0xbcd602u+id*388u)*4u));
            FUN_00489a80();
            id=U(0xd056f8);
            if(B(0xbcd606u+id*388u)==2)FUN_004b4980(id);
        }
        U(0xd056f8)++;
    } while(I(0xd056f8)<2000);
}
