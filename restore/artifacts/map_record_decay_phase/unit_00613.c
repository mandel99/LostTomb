#include "map_record_decay.h"
/* Original 00442ef0: no arguments or receiver; void cdecl result. */
void FUN_00442ef0(void)
{
    uint32_t i,record,timer,count=0,index;
    for(i=0,record=0x77f748u;i<200u;++i,record+=98u){
        if(MDR(record+1u,1)){
            timer=MDR(record+51u,1);
            if(timer)MDW(record+51u,1,timer-1u);
            if(MDR(record,1)==1u&&MDR(record+24u,1)==0u)++count;
        }
    }
    index=MDR(0x784f10u,4);
    /* Preserve the original signed branch: a zero count still stores one.
       Its negative arm is unreachable: this counter starts at zero and is <=200. */
    if((int32_t)count<0)MDW(0x7843dcu+index*32u,2,0);
    else MDW(0x7843dcu+index*32u,2,1);
    if(count!=1u){
        for(i=0,record=0x77f748u;i<200u;++i,record+=98u){
            if(MDR(record+1u,1)&&MDR(record,1)==1u&&MDR(record+24u,1)==0u&&MDR(record+51u,1)==0u)
                MDW(record+24u,1,4);
        }
    }
}
