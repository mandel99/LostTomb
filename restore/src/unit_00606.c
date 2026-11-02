#include "map_record_refresh.h"
/* Original 004425c0: one cdecl DWORD argument, full EAX result, no receiver. */
uint32_t FUN_004425c0(uint32_t record_index)
{
    uint32_t record=0x77f748u+record_index*98u,offset;
    MRE(record_index);
    switch(MRR(record+24u,1)){
    case 0:
        if(MRR(0x784b62u,2)!=10u)return MRR(0xf56444u,4);
        return MRR(0xf56954u,4);
    case 1:return MRR(0xf56954u,4);
    case 2:return MRR(0xf5693cu,4);
    case 3:return MRR(0xf56444u,4)+6u;
    case 4:return MRR(0xf56964u,4)+7u;
    case 5:
        offset=MRR(0xf56444u,4);
        return MRR(0x5d2a38u+MRR(record+25u,1)*8u,4)+offset;
    case 6:
        offset=MRR(0x5d2a38u+MRR(record+25u,1)*8u,4);
        return offset+MRR(0xf56964u,4);
    default:return 0;
    }
}
