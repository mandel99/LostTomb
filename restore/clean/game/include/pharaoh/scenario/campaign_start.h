#ifndef PHARAOH_SCENARIO_CAMPAIGN_START_H
#define PHARAOH_SCENARIO_CAMPAIGN_START_H

#include "pharaoh/runtime/settings_ini.h"

#include <stddef.h>
#include <stdint.h>

enum {
    PHARAOH_CAMPAIGN_INDEX_CAPACITY = 100,
    PHARAOH_CAMPAIGN_RECORD_BYTES = 64,
    PHARAOH_CAMPAIGN_RECORD_CHOICE_COUNT_OFFSET = 0x38,
    PHARAOH_CAMPAIGN_RECORD_CHOICE_VALUE_OFFSET = 0x2a,
    PHARAOH_CAMPAIGN_CITY_STATE_STRIDE_BYTES = 0x49d8,
    PHARAOH_CAMPAIGN_CITY_STATE_STRIDE_WORDS = 0x1276
};

typedef struct PharaohCampaignRecord {
    uint8_t bytes[PHARAOH_CAMPAIGN_RECORD_BYTES];
} PharaohCampaignRecord;

typedef struct PharaohCampaignIndex {
    PharaohCampaignRecord records[PHARAOH_CAMPAIGN_INDEX_CAPACITY];
    int32_t count;
} PharaohCampaignIndex;

_Static_assert(offsetof(PharaohCampaignIndex, count) == 0x1900,
               "campaign index count offset");
_Static_assert(PHARAOH_CAMPAIGN_CITY_STATE_STRIDE_WORDS * sizeof(uint32_t) ==
               PHARAOH_CAMPAIGN_CITY_STATE_STRIDE_BYTES,
               "campaign city-state byte and word stride");

typedef struct PharaohCampaignStartContext {
    const PharaohCampaignIndex *index;
    const int32_t *starting_favor_by_difficulty;
    PharaohSettingsFile *settings;
    uint32_t *city_state_words;
    uint32_t city_state_slot;
    uint8_t *campaign_status;
} PharaohCampaignStartContext;

int32_t pharaoh_campaign_find_mission(const PharaohCampaignIndex *index,
                                      uint32_t campaign_category,
                                      int32_t previous_index,
                                      int32_t choice_index);
uint8_t pharaoh_campaign_period_open(const PharaohCampaignIndex *index,
                                     uint32_t campaign_category);
uint8_t pharaoh_campaign_any_mission_played(const PharaohCampaignIndex *index);
uint8_t pharaoh_campaign_any_period_open(const PharaohCampaignIndex *index);
void pharaoh_settings_start_campaign(
    const PharaohCampaignStartContext *context, int8_t campaign_category);

#endif
