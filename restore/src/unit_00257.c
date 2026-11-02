#include "campaign_index.h"
uint8_t FUN_0041ba80(const RestoreCampaignIndex *self)
{
    int i;
    for(i=0;i<self->count;++i)if(self->records[i].bytes[0x3c])return 1;
    return 0;
}
