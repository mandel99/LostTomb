#ifndef PHARAOH_SCENARIO_CAMPAIGN_FILE_H
#define PHARAOH_SCENARIO_CAMPAIGN_FILE_H

#include "pharaoh/scenario/campaign_start.h"

typedef struct PharaohCampaignMissionRecord {
    int32_t score;
    int32_t mission;
    char player_name[32];
    int32_t rating_culture;
    int32_t rating_prosperity;
    int32_t rating_kingdom;
    int32_t population;
    int32_t treasury;
    int32_t months;
    int32_t difficulty;
    int32_t monuments;
    int32_t used;
} PharaohCampaignMissionRecord;

typedef struct PharaohCampaign {
    PharaohCampaignIndex index;
    char mission_names[100][50];
    int32_t mission_name_count;
    char saved_game_path[65];
    uint8_t reserved[3];
    PharaohCampaignMissionRecord mission_records[100];
} PharaohCampaign;

typedef struct PharaohCampaignTokenCursor {
    char *next;
} PharaohCampaignTokenCursor;

typedef struct PharaohCampaignFileOperations {
    void *user_data;
    void *(*open)(void *user_data, const char *filename, const char *mode,
                  uint32_t share_flags);
    char *(*read_line)(void *user_data, char *buffer, int32_t buffer_capacity,
                       void *stream);
    int32_t (*close)(void *user_data, void *stream);
    void (*report_message)(void *user_data, const char *message,
                           int32_t first_argument,
                           int32_t second_argument);
} PharaohCampaignFileOperations;

_Static_assert(sizeof(PharaohCampaignMissionRecord) == 0x4c,
               "campaign mission record layout");
_Static_assert(offsetof(PharaohCampaign, mission_name_count) == 0x2c8c &&
               offsetof(PharaohCampaign, saved_game_path) == 0x2c90 &&
               offsetof(PharaohCampaign, mission_records) == 0x2cd4 &&
               sizeof(PharaohCampaign) == 0x4a84,
               "campaign object native offsets and size");

void pharaoh_campaign_link_initialize(PharaohCampaignLink *link);
PharaohCampaign *pharaoh_campaign_initialize(PharaohCampaign *campaign);
uint8_t pharaoh_campaign_add_link(PharaohCampaignIndex *index,
                                  const PharaohCampaignLink *link);
void pharaoh_campaign_strip_control_bytes(char *text);
int32_t pharaoh_campaign_parse_integer(PharaohCampaignTokenCursor *cursor,
                                       int32_t *value);
int32_t pharaoh_campaign_parse_short(PharaohCampaignTokenCursor *cursor,
                                     int16_t *value);
void pharaoh_campaign_log_error(const PharaohCampaignFileOperations *operations,
                                const char *filename, int32_t line_number,
                                const char *source_line, const char *error);
int32_t pharaoh_campaign_load(PharaohCampaign *campaign,
                              const char *filename,
                              const PharaohCampaignFileOperations *operations);

#endif
