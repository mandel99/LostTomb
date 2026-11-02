#include "campaign_parser.h"
#include <string.h>
uint8_t FUN_0041ac90(RestoreCampaign *self,const RestoreCampaignRecord *record)
{
    if(self->index.count<0||self->index.count>=100||record->bytes[0]>=9)return 0;
    memcpy(&self->index.records[self->index.count++],record,sizeof(*record));return 1;
}
