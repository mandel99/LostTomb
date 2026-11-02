#include "map_validation.h"
uint32_t FUN_004437b0(uint32_t kind)
{
    uint32_t i,record=0x77f748u,state;
    MVE(0x4437b0,kind,0);
    for(i=0;i<200u;++i,record+=98u){
        if(MVR(record+1u,1)&&MVR(record,1)==1u){
            state=MVR(record+24u,1);
            if((state==1u||state==3u||state==5u)&&(state==1u||state==3u||state==5u))
                if(FUN_00443000(i,kind))return 1;
        }
    }
    return 0;
}
