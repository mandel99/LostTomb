#include "map_record_decay.h"
/* Original 00442f80: decrement precedes both counting and state conversion. */
void FUN_00442f80(void)
{
    uint32_t i,record,timer,state,count=0;
    for(i=0,record=0x77f748u;i<200u;++i,record+=98u){
        if(MDR(record+1u,1)){
            timer=MDR(record+51u,1);
            if(timer)MDW(record+51u,1,timer-1u);
            if(MDR(record,1)==1u){state=MDR(record+24u,1);if(state==1u||state==2u)++count;}
        }
    }
    if(count!=1u){
        for(i=0,record=0x77f748u;i<200u;++i,record+=98u){
            if(MDR(record+1u,1)&&MDR(record,1)==1u){
                state=MDR(record+24u,1);
                if((state==1u||state==2u)&&MDR(record+51u,1)==0u)MDW(record+24u,1,(state!=1u)+3u);
            }
        }
    }
}
