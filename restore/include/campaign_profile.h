#ifndef RESTORE_CAMPAIGN_PROFILE_H
#define RESTORE_CAMPAIGN_PROFILE_H
#include "campaign_parser.h"
/* Native profile size includes twelve words beyond the parsed campaign. */
typedef struct RestoreCampaignProfile {
    RestoreCampaign campaign;
    uint16_t suffix[12];
} RestoreCampaignProfile;
typedef char restore_campaign_profile_size[(sizeof(RestoreCampaignProfile)==0x4a9c)?1:-1];
#define RESTORE_CAMPAIGN_PROFILE ((RestoreCampaignProfile *)0x006fdd08u)
uint8_t FUN_0041b680(RestoreCampaignProfile *self,const char *path);
uint8_t FUN_0041b8a0(const RestoreCampaignProfile *self,const char *path);
#endif
