#include "pharaoh/scenario/campaign_start.h"

#include <string.h>

int32_t pharaoh_campaign_find_mission(const PharaohCampaignIndex *index,
                                      uint32_t campaign_category,
                                      int32_t previous_index,
                                      int32_t choice_index)
{
    uint8_t target_category = 0;
    if (previous_index >= 0) {
        if (index->count <= previous_index) {
            return -1;
        }

        const uint8_t *previous = index->records[previous_index].bytes;
        int32_t choice_count;
        memcpy(&choice_count,
            previous + PHARAOH_CAMPAIGN_RECORD_CHOICE_COUNT_OFFSET,
            sizeof(choice_count));
        if (choice_count > 0) {
            const uint32_t choice_offset =
                PHARAOH_CAMPAIGN_RECORD_CHOICE_VALUE_OFFSET +
                (uint32_t)choice_index * 12u;
            target_category = previous[choice_offset];
        } else {
            target_category = previous[1];
        }
    } else {
        previous_index = -1;
    }

    for (int32_t candidate = previous_index + 1;
         candidate < index->count; ++candidate) {
        const uint8_t *record = index->records[candidate].bytes;
        if (record[0] != campaign_category) {
            continue;
        }
        if (record[1] == 0 || record[1] == target_category) {
            return candidate;
        }
        for (uint32_t alternate = 0; alternate < record[7]; ++alternate) {
            if (record[alternate + 2] == target_category) {
                return candidate;
            }
        }
    }
    return -1;
}

void pharaoh_settings_start_campaign(
    const PharaohCampaignStartContext *context, int8_t campaign_category)
{
    PharaohSettingsFile *settings = context->settings;
    settings->starting_favor =
        context->starting_favor_by_difficulty[settings->difficulty];
    settings->campaign = (uint8_t)campaign_category;
    settings->personal_savings_last_mission = 0;
    settings->current_mission_id = 0;
    const uint32_t city_state_offset = context->city_state_slot *
        PHARAOH_CAMPAIGN_CITY_STATE_STRIDE_WORDS;
    context->city_state_words[city_state_offset] = 0;

    const int32_t selected = pharaoh_campaign_find_mission(context->index,
        (uint32_t)(int32_t)campaign_category, -1, 0);
    settings->campaign_link = (int8_t)(uint8_t)selected;
    const int32_t selected_index = settings->campaign_link;
    if (selected_index >= 0 && selected_index < context->index->count) {
        *context->campaign_status = 0;
        settings->mission_id = (int8_t)
            context->index->records[selected_index].bytes[12];
    } else {
        settings->mission_id = 0;
        *context->campaign_status = 0;
    }
    settings->previous_campaign_link = 0xff;
}
