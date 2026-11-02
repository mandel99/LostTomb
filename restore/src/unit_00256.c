#include "campaign_index.h"
uint8_t FUN_0041ba20(const RestoreCampaignIndex *self,uint32_t category)
{
    int index;
    if((int32_t)category>=5) return 1;
    for(index=0;index<self->count;++index) {
        const uint8_t *record=self->records[index].bytes;
        if((uint32_t)record[0]==category&&record[0x3c]&&FUN_0041b920(self,category,index,0)<0) return 1;
    }
    return 0;
}
