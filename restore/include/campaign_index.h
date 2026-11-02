#ifndef RESTORE_CAMPAIGN_INDEX_H
#define RESTORE_CAMPAIGN_INDEX_H
#include <stdint.h>
#include <stddef.h>
/* Verified prefix only. The parser's larger object is not modeled here. */
typedef struct RestoreCampaignRecord {uint8_t bytes[64];} RestoreCampaignRecord;
typedef struct RestoreCampaignIndex {RestoreCampaignRecord records[100];int32_t count;} RestoreCampaignIndex;
typedef char restore_campaign_count_offset[(offsetof(RestoreCampaignIndex,count)==0x1900)?1:-1];
#define RESTORE_CAMPAIGN_INDEX ((RestoreCampaignIndex *)0x006fdd08u)
int FUN_0041b920(const RestoreCampaignIndex *self,uint32_t category,int previous,int choice);
uint8_t FUN_0041ba20(const RestoreCampaignIndex *self,uint32_t category);
uint8_t FUN_0041bab0(const RestoreCampaignIndex *self);
uint8_t FUN_0041ba80(const RestoreCampaignIndex *self);
void FUN_00417dd0(int8_t category);
#endif
