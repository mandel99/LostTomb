#include "campaign_index.h"
#include <string.h>
int FUN_0041b920(const RestoreCampaignIndex *self,uint32_t category,int previous,int choice)
{
    uint8_t target=0;
    int index;
    if(previous<0) previous=-1;
    else {
        const uint8_t *record;
        int32_t choices;
        if(previous>=self->count) return -1;
        record=self->records[previous].bytes;
        memcpy(&choices,record+0x38,sizeof(choices));
        target=choices>0?*(const uint8_t *)((uintptr_t)record+0x2au+(uint32_t)choice*12u):record[1];
    }
    for(index=previous+1;index<self->count;++index) {
        const uint8_t *record=self->records[index].bytes;
        uint32_t i;
        if((uint32_t)record[0]!=category) continue;
        if(!record[1]||record[1]==target) return index;
        for(i=0;i<record[7];++i) if(record[2+i]==target) return index;
    }
    return -1;
}
