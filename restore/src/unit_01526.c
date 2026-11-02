#include "fleet_advisor_render.h"
/* Whole4BC620. Cache the full player before scanning1..1999. Native tests
   the alive BYTE for nonzero and compares the owner BYTE to that DWORD. */
int32_t FUN_004bc620(int32_t ordinal)
{
    uint32_t player=*(volatile uint32_t *)(uintptr_t)0xeaac28u,id,position=1;
    for(id=1;id<2000u;++id){
        volatile uint8_t *p=(volatile uint8_t *)(uintptr_t)(0xbcd602u+id*388u);
        if(p[4]&&p[0]==78u&&(uint32_t)p[5]==player){
            if(position==(uint32_t)ordinal)return (int32_t)id;
            ++position;
        }
    }
    return 0;
}
