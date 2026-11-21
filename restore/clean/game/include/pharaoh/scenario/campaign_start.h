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

typedef struct PharaohCampaignChoice {
    int32_t x;
    int32_t y;
    int16_t text_id;
    uint8_t path_id;
    uint8_t reserved;
} PharaohCampaignChoice;

typedef struct PharaohCampaignLink {
    uint8_t period;
    uint8_t path_id;
    uint8_t merge_ids[5];
    uint8_t merge_count;
    int32_t intro_id;
    int32_t mission_id;
    int32_t transition_text_id;
    int32_t reserved1;
    int32_t text_id;
    int32_t background_id;
    PharaohCampaignChoice choices[2];
    int32_t choice_count;
    uint8_t played;
    uint8_t reserved2[3];
} PharaohCampaignLink;

typedef union PharaohCampaignRecord {
    uint8_t bytes[PHARAOH_CAMPAIGN_RECORD_BYTES];
    PharaohCampaignLink link;
} PharaohCampaignRecord;

typedef struct PharaohCampaignIndex {
    PharaohCampaignRecord records[PHARAOH_CAMPAIGN_INDEX_CAPACITY];
    int32_t count;
} PharaohCampaignIndex;

_Static_assert(offsetof(PharaohCampaignIndex, count) == 0x1900,
               "campaign index count offset");
_Static_assert(sizeof(PharaohCampaignChoice) == 0x0c,
               "campaign choice layout");
_Static_assert(sizeof(PharaohCampaignLink) == PHARAOH_CAMPAIGN_RECORD_BYTES,
               "campaign link layout");
_Static_assert(offsetof(PharaohCampaignLink, choices) == 0x20 &&
               offsetof(PharaohCampaignLink, choice_count) == 0x38 &&
               offsetof(PharaohCampaignLink, played) == 0x3c,
               "campaign link native field offsets");
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
/* Complete typed C translation of native 0x0041BAE0. */
void pharaoh_campaign_unlock_all(PharaohCampaignIndex *index);
void pharaoh_settings_start_campaign(
    const PharaohCampaignStartContext *context, int8_t campaign_category);

#endif
