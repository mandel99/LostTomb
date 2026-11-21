#include "pharaoh/scenario/campaign_start.h"

uint8_t pharaoh_campaign_period_open(const PharaohCampaignIndex *index,
                                     uint32_t campaign_category)
{
    if ((int32_t)campaign_category > 4) {
        return UINT8_C(1);
    }
    for (int32_t mission = 0; mission < index->count; ++mission) {
        const uint8_t *record = index->records[mission].bytes;
        if (record[0] == campaign_category && record[0x3c] != 0 &&
            pharaoh_campaign_find_mission(index, campaign_category,
                mission, 0) < 0) {
            return UINT8_C(1);
        }
    }
    return UINT8_C(0);
}

uint8_t pharaoh_campaign_any_mission_played(const PharaohCampaignIndex *index)
{
    for (int32_t mission = 0; mission < index->count; ++mission) {
        if (index->records[mission].bytes[0x3c] != 0) {
            return UINT8_C(1);
        }
    }
    return UINT8_C(0);
}

uint8_t pharaoh_campaign_any_period_open(const PharaohCampaignIndex *index)
{
    for (uint32_t category = 0; category < 9; ++category) {
        if (pharaoh_campaign_period_open(index, category) != 0) {
            return UINT8_C(1);
        }
    }
    return UINT8_C(0);
}

void pharaoh_campaign_unlock_all(PharaohCampaignIndex *index)
{
    int32_t mission = 0;
    if (index->count <= 0) return;
    do {
        index->records[mission].link.played = 1;
        ++mission;
    } while (mission < index->count);
}
