#include "animation_state.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define H(a) (*(int16_t *)(uintptr_t)(a))
void FUN_004e5a80(void)
{
    int32_t id=FUN_004e59c0();
    uint32_t index=0,kind_address;
    if(!id)return;
    kind_address=0x7999e8u+(uint32_t)id*264u;
    while(index<(H(kind_address)==0xcf?10u:18u)) {
        uint32_t table=H(kind_address)==0xcf?0x586b08u:0x58fe48u;
        FUN_0041fdb0((void *)(uintptr_t)U(table+8u+index*16u));
        ++index;
    }
}
