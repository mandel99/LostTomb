#include "pharaoh/scenario/campaign_file.h"

#include <stdio.h>
#include <string.h>

enum {
    PHARAOH_CAMPAIGN_PERIOD_COUNT = 9,
    PHARAOH_CAMPAIGN_LINE_BYTES = 200,
    PHARAOH_CAMPAIGN_NAME_BYTES = 50,
    PHARAOH_CAMPAIGN_ERROR_BYTES = 300,
    PHARAOH_CAMPAIGN_FILE_SHARE_DENY_NONE = 0x40,
    PHARAOH_CAMPAIGN_FIRST_CHOICE_COORDINATE = 50,
    PHARAOH_CAMPAIGN_CHOICE_COORDINATE_STEP = 50
};

static const char *const campaign_section_names[13] = {
    "[MISSION_NAMES]",
    "[PREDYNASTIC_PERIOD]",
    "[ARCHAIC_PERIOD]",
    "[OLD_KINGDOM]",
    "[MIDDLE_KINGDOM]",
    "[NEW_KINGDOM]",
    "[VALLEY_OF_THE_KINGS]",
    "[RAMSES_II]",
    "[ANCIENT_CONQUERORS]",
    "[CLEOPATRAS_ALEXANDRIA]",
    "mission",
    "choicescreen",
    "choice"
};

static const char *const campaign_period_names[9] = {
    "[PREDYNASTIC_PERIOD]",
    "[ARCHAIC_PERIOD]",
    "[OLD_KINGDOM]",
    "[MIDDLE_KINGDOM]",
    "[NEW_KINGDOM]",
    "[VALLEY_OF_THE_KINGS]",
    "[RAMSES_II]",
    "[ANCIENT_CONQUERORS]",
    "[CLEOPATRAS_ALEXANDRIA]"
};

static const char campaign_file_mode[] = "rb";

static char *campaign_next_token(PharaohCampaignTokenCursor *cursor,
                                 char *input)
{
    char *token;
    char *end;

    if (input != NULL) {
        cursor->next = input;
    }
    if (cursor->next == NULL) {
        return NULL;
    }

    token = cursor->next;
    while (*token == ',' || *token == '=') {
        ++token;
    }
    if (*token == '\0') {
        cursor->next = token;
        return NULL;
    }

    end = token;
    while (*end != '\0' && *end != ',' && *end != '=') {
        ++end;
    }
    if (*end != '\0') {
        *end = '\0';
        cursor->next = end + 1;
    } else {
        cursor->next = end;
    }
    return token;
}

static int32_t campaign_section_index(const char *token)
{
    for (int32_t index = 0;
         index < (int32_t)(sizeof(campaign_section_names) /
                           sizeof(campaign_section_names[0])); ++index) {
#if defined(_MSC_VER)
        if (_stricmp(token, campaign_section_names[index]) == 0) {
            return index;
        }
#else
        const unsigned char *left = (const unsigned char *)token;
        const unsigned char *right =
            (const unsigned char *)campaign_section_names[index];
        while (*left != '\0' && *right != '\0') {
            unsigned char left_byte = *left++;
            unsigned char right_byte = *right++;
            if (left_byte >= 'A' && left_byte <= 'Z') {
                left_byte = (unsigned char)(left_byte + ('a' - 'A'));
            }
            if (right_byte >= 'A' && right_byte <= 'Z') {
                right_byte = (unsigned char)(right_byte + ('a' - 'A'));
            }
            if (left_byte != right_byte) {
                break;
            }
        }
        if (*left == '\0' && *right == '\0') {
            return index;
        }
#endif
    }
    return -1;
}

static void campaign_reset_pending_link_fields(PharaohCampaignLink *link)
{
    link->merge_count = 0;
    link->intro_id = -1;
    link->path_id = 0;
    link->mission_id = -1;
    link->transition_text_id = -1;
    link->choice_count = 0;
}

static void campaign_report_parse_error(
    const PharaohCampaignFileOperations *operations, const char *filename,
    int32_t line_number, const char *source_line, const char *error)
{
    pharaoh_campaign_log_error(operations, filename, line_number, source_line,
        error);
}

void pharaoh_campaign_link_initialize(PharaohCampaignLink *link)
{
    /* Native leaves merge IDs and choice padding untouched. */
    link->period = UINT8_C(0xff);
    link->path_id = 0;
    link->merge_count = 0;
    link->intro_id = -1;
    link->mission_id = -1;
    link->transition_text_id = -1;
    link->reserved1 = -1;
    link->text_id = -1;
    link->background_id = -1;

    for (uint32_t choice = 0; choice < 2; ++choice) {
        const int32_t coordinate =
            PHARAOH_CAMPAIGN_FIRST_CHOICE_COORDINATE +
            (int32_t)choice * PHARAOH_CAMPAIGN_CHOICE_COORDINATE_STEP;
        link->choices[choice].x = coordinate;
        link->choices[choice].y = coordinate;
        link->choices[choice].text_id = -1;
        link->choices[choice].path_id = 0;
    }

    link->choice_count = 0;
    link->played = 0;
}

PharaohCampaign *pharaoh_campaign_initialize(PharaohCampaign *campaign)
{
    for (uint32_t index = 0; index < PHARAOH_CAMPAIGN_INDEX_CAPACITY; ++index) {
        pharaoh_campaign_link_initialize(&campaign->index.records[index].link);
    }

    campaign->index.count = 0;
    campaign->mission_name_count = 0;
    /* The native initializer clears exactly the 65-byte path field. */
    memset(campaign->saved_game_path, 0, sizeof(campaign->saved_game_path));
    return campaign;
}

uint8_t pharaoh_campaign_add_link(PharaohCampaignIndex *index,
                                  const PharaohCampaignLink *link)
{
    if (index->count >= PHARAOH_CAMPAIGN_INDEX_CAPACITY ||
        link->period >= PHARAOH_CAMPAIGN_PERIOD_COUNT) {
        return 0;
    }

    index->records[index->count].link = *link;
    ++index->count;
    return 1;
}

void pharaoh_campaign_strip_control_bytes(char *text)
{
    char *read = text;
    char *write = text;

    while (*read != '\0') {
        if ((uint8_t)*read >= UINT8_C(0x21)) {
            *write = *read;
            ++write;
        }
        ++read;
    }

    *write = '\0';
}

int32_t pharaoh_campaign_parse_integer(PharaohCampaignTokenCursor *cursor,
                                       int32_t *value)
{
    char *token = campaign_next_token(cursor, NULL);
    char *digit = token;
    uint32_t number = 0;
    uint32_t result_bits;
    int negative = 0;

    if (token == NULL) {
        return 0;
    }
    if (*digit == '-') {
        negative = 1;
        ++digit;
    }
    while (*digit != '\0') {
        if (*digit < '0' || *digit > '9') {
            return 2;
        }
        number = number * UINT32_C(10) +
            (uint32_t)((unsigned char)*digit - (unsigned char)'0');
        ++digit;
    }

    result_bits = negative != 0 ? UINT32_C(0) - number : number;
    memcpy(value, &result_bits, sizeof(result_bits));
    return 1;
}

int32_t pharaoh_campaign_parse_short(PharaohCampaignTokenCursor *cursor,
                                     int16_t *value)
{
    int32_t parsed_value;
    const int32_t result =
        pharaoh_campaign_parse_integer(cursor, &parsed_value);
    if (result == 1) {
        const uint16_t low_word = (uint16_t)(uint32_t)parsed_value;
        memcpy(value, &low_word, sizeof(low_word));
    }
    return result;
}

void pharaoh_campaign_log_error(
                                const PharaohCampaignFileOperations *operations,
                                const char *filename, int32_t line_number,
                                const char *source_line, const char *error)
{
    char message[PHARAOH_CAMPAIGN_ERROR_BYTES];
    (void)snprintf(message, sizeof(message), "ERR: %s line %d: %s %s", filename,
        line_number, source_line, error);
    operations->report_message(operations->user_data, message, 0, 0);
}

int32_t pharaoh_campaign_load(PharaohCampaign *campaign,
                              const char *filename,
                              const PharaohCampaignFileOperations *operations)
{
    PharaohCampaignLink pending_link;
    PharaohCampaignTokenCursor token_cursor = { NULL };
    char line[PHARAOH_CAMPAIGN_LINE_BYTES];
    char original_line[PHARAOH_CAMPAIGN_LINE_BYTES];
    char generated_error[PHARAOH_CAMPAIGN_LINE_BYTES];
    int32_t line_number = 0;
    uint8_t mission_names_section = 0;
    uint8_t succeeded = 1;

    pharaoh_campaign_link_initialize(&pending_link);
    void *file = operations->open(operations->user_data, filename,
        campaign_file_mode, PHARAOH_CAMPAIGN_FILE_SHARE_DENY_NONE);
    if (file == NULL) {
        return 0;
    }

    (void)pharaoh_campaign_initialize(campaign);
    while (operations->read_line(operations->user_data, line,
            (int32_t)sizeof(line), file) != NULL) {
        const size_t line_bytes = strlen(line) + 1;
        memcpy(original_line, line, line_bytes);
        ++line_number;
        pharaoh_campaign_strip_control_bytes(line);
        if (line[0] == ';' || line[0] == '\0') {
            continue;
        }

        char *token = campaign_next_token(&token_cursor, line);
        if (token == NULL) {
            continue;
        }
        const int32_t section = campaign_section_index(token);
        if (section == 0) {
            mission_names_section = 1;
            continue;
        }
        if (section >= 1 && section <= PHARAOH_CAMPAIGN_PERIOD_COUNT) {
            mission_names_section = 0;
            if (pending_link.mission_id >= 0) {
                if (pharaoh_campaign_add_link(&campaign->index,
                        &pending_link) == 0) {
                    campaign_report_parse_error(operations, filename, line_number,
                        original_line, "failed to add mission link");
                    succeeded = 0;
                    continue;
                }
                campaign_reset_pending_link_fields(&pending_link);
            }
            pending_link.period = (uint8_t)(section - 1);
            continue;
        }

        if (section == 10) {
            int32_t parse_result;
            int32_t parsed_value;

            if (mission_names_section != 0) {
                campaign_report_parse_error(operations, filename, line_number,
                    original_line, "bad mission name");
                succeeded = 0;
                continue;
            }
            if (pending_link.mission_id >= 0) {
                if (pharaoh_campaign_add_link(&campaign->index,
                        &pending_link) == 0) {
                    campaign_report_parse_error(operations, filename, line_number,
                        original_line, "failed to add mission link");
                    succeeded = 0;
                    continue;
                }
                campaign_reset_pending_link_fields(&pending_link);
            }

            parse_result = pharaoh_campaign_parse_integer(&token_cursor,
                &pending_link.mission_id);
            if (parse_result != 1 || pending_link.mission_id < 0 ||
                pending_link.mission_id >= campaign->mission_name_count) {
                campaign_report_parse_error(operations, filename, line_number,
                    original_line, "bad mission id");
                succeeded = 0;
                continue;
            }

            parse_result = pharaoh_campaign_parse_integer(&token_cursor,
                &pending_link.intro_id);
            if (parse_result != 1) {
                campaign_report_parse_error(operations, filename, line_number,
                    original_line, "bad intro id");
                succeeded = 0;
                continue;
            }
            parse_result = pharaoh_campaign_parse_integer(&token_cursor,
                &pending_link.transition_text_id);
            if (parse_result != 1) {
                campaign_report_parse_error(operations, filename, line_number,
                    original_line, "bad transition text id");
                succeeded = 0;
                continue;
            }
            parse_result = pharaoh_campaign_parse_integer(&token_cursor,
                &parsed_value);
            if (parse_result == 2) {
                campaign_report_parse_error(operations, filename, line_number,
                    original_line, "bad path_id");
                succeeded = 0;
                continue;
            }
            if (parse_result == 1) {
                if (parsed_value < 0 || parsed_value > UINT8_MAX) {
                    campaign_report_parse_error(operations, filename, line_number,
                        original_line, "bad path_id");
                    succeeded = 0;
                    continue;
                }
                pending_link.path_id = (uint8_t)parsed_value;
                pending_link.merge_count = 0;
                for (uint32_t merge = 0; merge < 5; ++merge) {
                    if (campaign_next_token(&token_cursor, NULL) == NULL) {
                        break;
                    }
                    parse_result = pharaoh_campaign_parse_integer(
                        &token_cursor, &parsed_value);
                    if (parse_result == 0) {
                        break;
                    }
                    if (parse_result == 2 || parsed_value < 0 ||
                        parsed_value > UINT8_MAX) {
                        campaign_report_parse_error(operations, filename,
                            line_number, original_line, "bad merge id");
                        succeeded = 0;
                        break;
                    }
                    pending_link.merge_ids[merge] = (uint8_t)parsed_value;
                    ++pending_link.merge_count;
                }
            }
            continue;
        }

        if (section == 11) {
            int32_t parse_result;
            if (pending_link.mission_id < 0) {
                pending_link.mission_id = 0x7f;
            }
            if (mission_names_section != 0) {
                campaign_report_parse_error(operations, filename, line_number,
                    original_line, "bad mission name");
                succeeded = 0;
                continue;
            }
            parse_result = pharaoh_campaign_parse_integer(&token_cursor,
                &pending_link.background_id);
            if (parse_result != 1) {
                campaign_report_parse_error(operations, filename, line_number,
                    original_line, "bad background id");
                succeeded = 0;
                continue;
            }
            parse_result = pharaoh_campaign_parse_integer(&token_cursor,
                &pending_link.text_id);
            if (parse_result != 1) {
                campaign_report_parse_error(operations, filename, line_number,
                    original_line, "bad text id");
                succeeded = 0;
            }
            continue;
        }

        if (section == 12) {
            int32_t parsed_value;
            int32_t parse_result;
            if (mission_names_section != 0) {
                campaign_report_parse_error(operations, filename, line_number,
                    original_line, "bad mission name");
                succeeded = 0;
                continue;
            }
            if (pending_link.choice_count >= 2) {
                campaign_report_parse_error(operations, filename, line_number,
                    original_line, "too many choices");
                succeeded = 0;
                continue;
            }

            PharaohCampaignChoice *choice =
                &pending_link.choices[pending_link.choice_count];
            parse_result = pharaoh_campaign_parse_integer(&token_cursor,
                &parsed_value);
            if (parse_result != 1 || parsed_value < 0 ||
                parsed_value > UINT8_MAX) {
                campaign_report_parse_error(operations, filename, line_number,
                    original_line, "bad path_id");
                succeeded = 0;
                continue;
            }
            choice->path_id = (uint8_t)parsed_value;
            parse_result = pharaoh_campaign_parse_integer(&token_cursor,
                &choice->x);
            if (parse_result != 1) {
                campaign_report_parse_error(operations, filename, line_number,
                    original_line, "bad x value");
                succeeded = 0;
                continue;
            }
            parse_result = pharaoh_campaign_parse_integer(&token_cursor,
                &choice->y);
            if (parse_result != 1) {
                campaign_report_parse_error(operations, filename, line_number,
                    original_line, "bad y value");
                succeeded = 0;
                continue;
            }
            parse_result = pharaoh_campaign_parse_short(&token_cursor,
                &choice->text_id);
            if (parse_result != 1) {
                campaign_report_parse_error(operations, filename, line_number,
                    original_line, "bad text id");
                succeeded = 0;
                continue;
            }
            ++pending_link.choice_count;
            continue;
        }

        if (mission_names_section == 0) {
            campaign_report_parse_error(operations, filename, line_number,
                original_line, "unrecognized line");
            succeeded = 0;
            continue;
        }

        int32_t last_character = (int32_t)strlen(original_line) - 1;
        while (last_character >= 0 &&
               (uint8_t)original_line[last_character] < UINT8_C(0x21)) {
            original_line[last_character] = '\0';
            --last_character;
        }
        if (last_character + 2 < PHARAOH_CAMPAIGN_NAME_BYTES + 1) {
            if (last_character > 0) {
                if (campaign->mission_name_count > 99) {
                    campaign_report_parse_error(operations, filename, line_number,
                        original_line, "max missions exceeded");
                    succeeded = 0;
                    continue;
                }
                memcpy(campaign->mission_names[campaign->mission_name_count],
                    original_line, (size_t)last_character + 2);
                ++campaign->mission_name_count;
            }
        } else {
            campaign_report_parse_error(operations, filename, line_number,
                original_line, "mission name too long");
            succeeded = 0;
        }
    }

    if (pending_link.mission_id >= 0) {
        if (pharaoh_campaign_add_link(&campaign->index, &pending_link) == 0) {
            campaign_report_parse_error(operations, filename, line_number,
                original_line, "failed to add mission link");
            succeeded = 0;
        }
    }

    for (int32_t link_index = 0; link_index < campaign->index.count;
         ++link_index) {
        const PharaohCampaignLink *link =
            &campaign->index.records[link_index].link;
        if (link->choice_count > 1) {
            for (int32_t choice_index = 0;
                 choice_index < link->choice_count; ++choice_index) {
                if (pharaoh_campaign_find_mission(&campaign->index,
                        link->period, link_index, choice_index) < 0) {
                    (void)snprintf(generated_error, sizeof(generated_error),
                        "%s mission %d: choice %d not found",
                        campaign_period_names[link->period],
                        link->mission_id,
                        link->choices[choice_index].path_id);
                    campaign_report_parse_error(operations, filename, line_number,
                        "", generated_error);
                    succeeded = 0;
                }
            }
        }
    }

    memset(campaign->mission_records, 0, sizeof(campaign->mission_records));
    (void)operations->close(operations->user_data, file);
    return succeeded;
}
