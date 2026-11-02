#include "order_actions_finish.h"
int32_t FUN_004b55f0(uint32_t tile)
{
    int32_t id=OA_S(0xb4e6fcu+tile*2u);uint32_t record;int16_t kind;
    if(id<=0)return 0;
    record=0x7999d8u+(uint32_t)id*264u;
    if(OA_B(record)!=1)return 0;
    kind=OA_S(record+16u);
    if(kind==0x36||kind==0x39)return OA_S(record+80u);
    return 0;
}
