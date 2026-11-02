#include "map_validation.h"
uint32_t FUN_00443000(uint32_t record_index,uint32_t kind)
{
    uint32_t i,base=0x77f766u+record_index*98u;
    MVE(0x443000,record_index,kind);
    for(i=0;i<14u;++i)if(MVR(base+i,1)==kind)return 1;
    return 0;
}
