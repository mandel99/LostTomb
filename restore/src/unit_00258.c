#include "campaign_index.h"
uint8_t FUN_0041bab0(const RestoreCampaignIndex *self)
{
    uint32_t category;
    for(category=0;category<9;++category) if(FUN_0041ba20(self,category)) return 1;
    return 0;
}
