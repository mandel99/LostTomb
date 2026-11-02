#include "campaign_parser.h"
#include <string.h>
RestoreCampaign *FUN_0041ac50(RestoreCampaign *self)
{
    int i;
    for(i=0;i<100;++i)FUN_0041ac00(&self->index.records[i]);
    self->index.count=0;self->name_count=0;memset(self->active,0,sizeof(self->active));
    return self;
}
