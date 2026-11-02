#include "mausoleum.h"
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))

/* Whole4E0C80: refresh every part, but only when the selected building's
   full WORD kind is 222. Piece index is a signed WORD, not a DWORD. */
void FUN_004e0c80(int32_t building)
{
    uint32_t root,part,offset=(uint32_t)building*264u;
    if(W(0x7999e8u+offset)!=222u)return;
    root=(uint32_t)building-(uint32_t)(int32_t)(int16_t)W(0x7999eau+offset);
    for(part=0;part<116u;++part)FUN_004dfbb0((int32_t)(root+part));
}
