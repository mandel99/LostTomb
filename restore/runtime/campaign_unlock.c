#include "campaign_unlock.h"
/* Whole native 0041BAE0: signed count at +0x1900, BYTE flag at +0x3c,
 * 64-byte record stride. Reload count after each flag store as native does. */
void restore_campaign_unlock_all(RestoreCampaignIndex *self)
{
    volatile RestoreCampaignIndex *observed=self;
    int32_t index=0,count;
    uint32_t flag=(uint32_t)(uintptr_t)self+0x3cu;
    if(observed->count<=0)return;
    do {
        *(volatile uint8_t *)(uintptr_t)flag=1;
        count=observed->count;
        index=(int32_t)((uint32_t)index+1u);
        flag+=64u;
    } while(index<count);
}
