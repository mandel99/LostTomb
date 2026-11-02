#include "camera_bounds.h"
#include "restore_types.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern bool FUN_0047e830(int32_t tile);
/* Lookup entries and all four limits are DWORD values. */
void FUN_004ddf90(void)
{
    uint32_t address=0x741b84u,tile=U(address);
    int32_t index=0;
    do {
        if(!FUN_0047e830((int32_t)tile)) break;
        address+=4;tile=U(address);++index;
    } while(address<0x741f1cu);
    U(0xe94dd0)=index+1u;
    address=0x741f1cu;tile=U(address);index=230;
    do {
        if(!FUN_0047e830((int32_t)tile)) break;
        address-=4;tile=U(address);--index;
    } while(address>0x741b84u);
    U(0xe94dcc)=(uint32_t)index-1u;
    address=0x70e660u;tile=U(address);index=0;
    do {
        if(!FUN_0047e830((int32_t)tile)) break;
        address+=0x738u;tile=U(address);index+=2;
    } while(address<0x775440u);
    U(0xe94dc8)=index+1u;
    address=0x774d08u;tile=U(address);index=454;
    do {
        if(!FUN_0047e830((int32_t)tile)) break;
        address-=0x738u;tile=U(address);index-=2;
    } while(address>0x70e9fcu);
    U(0xe94dc8)&=0xfffeu;
    U(0xe94dc4)=((uint32_t)index-1u)&0xfffeu;
}
