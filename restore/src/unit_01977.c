#include "animation_state.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define H(a) (*(int16_t *)(uintptr_t)(a))
int32_t FUN_004e59c0(void)
{
    uint32_t player=U(0xeaac28),i;
    for(i=0;i<3;++i) {
        int32_t id=(int32_t)U(0xea5facu+player*0x49d8u+i*4u);
        if(id) {
            uint32_t building=0x7999d8u+(uint32_t)id*264u;
            int16_t kind=H(building+16);
            if(B(building)==1 && (kind==0xcf||kind==0xd0) && B(building+0x55)!=100)
                return id;
        }
    }
    return 0;
}
