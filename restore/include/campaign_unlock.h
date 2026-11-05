#ifndef RESTORE_CAMPAIGN_UNLOCK_H
#define RESTORE_CAMPAIGN_UNLOCK_H
#include "campaign_index.h"
/* Native 0041BAE0 receives this in ECX and has no logical scalar result. */
void restore_campaign_unlock_all(RestoreCampaignIndex *self);
void FUN_0041bae0(void);
#endif
