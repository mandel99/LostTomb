#include "map_validation.h"
uint32_t FUN_00443820(uint32_t kind)
{
    uint32_t i,record=0x77f748u,remapped;
    MVE(0x443820,kind,0);
    for(i=0;i<200u;++i,record+=98u){
        if(MVR(record+1u,1)&&MVR(record,1)==1u&&MVR(record+24u,1)==0u){
            remapped=1;
            switch(kind){
            case 15:kind=14;break;case 17:kind=16;break;case 33:kind=32;break;
            case 19:kind=18;break;case 13:kind=11;break;case 10:kind=29;break;
            case 28:kind=20;break;case 23:kind=22;break;default:remapped=0;break;
            }
            /* Native ESI retains the mapped kind for subsequent records. */
            if(remapped&&FUN_004437b0(kind))return 1;
            if(FUN_00443000(i,kind))return 1;
        }
    }
    return 0;
}
