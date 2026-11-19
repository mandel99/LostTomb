#include "pharaoh/scenario/campaign_profile_load.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

enum {
    PROFILE_BYTES = 0x4a9c,
    PROFILE_FALLBACK_BYTES = 0x4a6c,
    PROFILE_UNREAD_TAIL_BYTES = PROFILE_BYTES - PROFILE_FALLBACK_BYTES,
    PROFILE_STATE_OFFSET = 0x2cd4,
    PROFILE_STATE_BYTES = 0x1db0,
    PROFILE_SAVED_PATH_BYTES = 65,
    PROFILE_RECORD_BYTES = 0x4c,
    PROFILE_RECORD_USED_OFFSET = 0x48,
    PROFILE_COMPLETED_OFFSET = 0x3c
};

typedef struct PharaohCampaignProfileFile {
    PharaohCampaign campaign;
    uint16_t suffix[PHARAOH_CAMPAIGN_PROFILE_SUFFIX_WORDS];
} PharaohCampaignProfileFile;

_Static_assert(sizeof(PharaohCampaignProfileFile) == PROFILE_BYTES,
    "campaign profile file size");
_Static_assert(offsetof(PharaohCampaignProfileFile, campaign.mission_records) ==
        PROFILE_STATE_OFFSET, "campaign profile state offset");
_Static_assert(sizeof(((PharaohCampaign *)0)->mission_records) ==
        PROFILE_STATE_BYTES, "campaign profile state size");
_Static_assert(PROFILE_UNREAD_TAIL_BYTES == 0x30,
    "campaign profile fallback tail size");
_Static_assert(sizeof(((PharaohCampaign *)0)->mission_records[0]) ==
        PROFILE_RECORD_BYTES &&
    offsetof(PharaohCampaignMissionRecord, used) == PROFILE_RECORD_USED_OFFSET,
    "campaign profile completion field offset");

int32_t pharaoh_campaign_profile_load(const PharaohFileReadContext *files,
    PharaohCampaign *campaign,
    uint16_t suffix[PHARAOH_CAMPAIGN_PROFILE_SUFFIX_WORDS],
    const char *path)
{
    PharaohCampaignProfileFile profile = { 0 };
    PharaohReadOnlyFile *file;
    size_t read_count;
    size_t path_length;

    pharaoh_campaign_initialize(&profile.campaign);
    memset(suffix, 0,
        PHARAOH_CAMPAIGN_PROFILE_SUFFIX_WORDS * sizeof(suffix[0]));
    file = files->operations->open(files->user_data, path);
    if (file == NULL) return 0;

    read_count = files->operations->read(files->user_data, &profile,
        PROFILE_BYTES, 1, file);
    if (read_count == 0) {
        (void)files->operations->seek(files->user_data, file, 0, 0);
        read_count = files->operations->read(files->user_data, &profile,
            PROFILE_FALLBACK_BYTES, 1, file);
    } else {
        memcpy(suffix, profile.suffix,
            PHARAOH_CAMPAIGN_PROFILE_SUFFIX_WORDS * sizeof(suffix[0]));
    }
    files->operations->close(files->user_data, file);
    if (read_count != 1 || campaign->mission_name_count < 0 ||
        campaign->mission_name_count > PHARAOH_CAMPAIGN_INDEX_CAPACITY ||
        memchr(profile.campaign.saved_game_path, '\0',
            PROFILE_SAVED_PATH_BYTES) == NULL) {
        return 0;
    }

    path_length = strlen(profile.campaign.saved_game_path) + 1;
    memcpy(campaign->saved_game_path, profile.campaign.saved_game_path,
        path_length);
    memcpy(campaign->mission_records, profile.campaign.mission_records,
        PROFILE_STATE_BYTES);
    for (int32_t index = 0; index < campaign->mission_name_count; ++index) {
        campaign->index.records[index].bytes[PROFILE_COMPLETED_OFFSET] =
            (uint8_t)(campaign->mission_records[index].used != 0);
    }
    return 1;
}

int32_t pharaoh_campaign_profile_save(const PharaohCampaign *campaign,
    const uint16_t suffix[PHARAOH_CAMPAIGN_PROFILE_SUFFIX_WORDS],
    const char *path)
{
    PharaohCampaignProfileFile profile;
    FILE *file = NULL;
    size_t written;

    profile.campaign = *campaign;
    memcpy(profile.suffix, suffix, sizeof(profile.suffix));
#if defined(_MSC_VER)
    if (fopen_s(&file, path, "wb") != 0 || file == NULL) return 0;
#else
    file = fopen(path, "wb");
    if (file == NULL) return 0;
#endif
    written = fwrite(&profile, sizeof(profile), 1, file);
    (void)fclose(file);
    return written == 1;
}
