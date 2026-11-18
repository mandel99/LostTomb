#ifndef PHARAOH_SCENARIO_CAMPAIGN_PROFILE_LOAD_H
#define PHARAOH_SCENARIO_CAMPAIGN_PROFILE_LOAD_H

#include "pharaoh/runtime/file_loading.h"
#include "pharaoh/scenario/campaign_file.h"

#include <stdint.h>

enum { PHARAOH_CAMPAIGN_PROFILE_SUFFIX_WORDS = 12 };

/* Native player-profile loader 0x0041B680. */
int32_t pharaoh_campaign_profile_load(const PharaohFileReadContext *files,
    PharaohCampaign *campaign,
    uint16_t suffix[PHARAOH_CAMPAIGN_PROFILE_SUFFIX_WORDS],
    const char *path);

#endif
