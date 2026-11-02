#include "map_record_refresh.h"
/* Original 004426c0: refresh exactly 200 records; lookup indices use low AX. */
void FUN_004426c0(void)
{
    uint32_t i,record,index,limit,xindex,yindex;
    for(i=0,record=0x77f748u;i<200u;++i,record+=98u){
        if(MRR(record+1u,1)&&MRR(record,1)==1u){
            index=FUN_004425c0(i);
            limit=MRR(0x126517cu,4);
            MRW16(record+12u,index);
            index&=65535u;
            xindex=(int32_t)index<(int32_t)limit?index:0;
            MRW16(record+8u,MRR(0x10d6a0cu+xindex*64u,2));
            yindex=(int32_t)index<(int32_t)limit?index:0;
            MRW16(record+10u,MRR(0x10d6a0eu+yindex*64u,2));
        }
    }
}
