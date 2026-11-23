#ifndef PHARAOH_UI_CITY_MESSAGE_TEXT_H
#define PHARAOH_UI_CITY_MESSAGE_TEXT_H

#include <stdint.h>

#include "pharaoh/ui/message_actions.h"

enum {
    PHARAOH_CITY_MESSAGE_RECORD_CAPACITY = 1000
};

typedef uint32_t (*PharaohCityMessageGetTime)(void *user_data);

typedef struct PharaohCityMessageProblemState {
    uint32_t last_update_time;
    int32_t problem_count;
    int32_t problem_area_b;
} PharaohCityMessageProblemState;

/* Native004E1770. Serialized counters and process-owned UI/queue state
 * remain separate pointers; no duplicated snapshot of a saved counter. */
typedef struct PharaohCityMessageScenarioReset {
    PharaohMenuMessageRecord *messages; /*1000 records of48 bytes*/
    uint8_t *population_shown; /*10 bytes*/
    int32_t *counts, *delays, *queue; /*20 DWORDs each*/
    int32_t *messages_used, *queue_delay, *next_sequence;
    int32_t *current_id, *shown_index, *scroll_position, *list_scroll;
    int32_t *posted_once, *fire_last_time, *collapse_last_time;
    int32_t *reserved1, *reserved2, *count_problem, *queue_reserved;
    PharaohCityMessageProblemState *problem;
    PharaohCityMessageGetTime get_time;
    void *clock_user_data;
} PharaohCityMessageScenarioReset;

void pharaoh_city_message_initialize_scenario(
    const PharaohCityMessageScenarioReset *state);

int32_t pharaoh_city_message_text_id(int32_t message_id);
void pharaoh_city_message_set_category(uint32_t message,
    uint32_t *category);
int32_t pharaoh_city_message_new_id(
    const PharaohMenuMessageRecord messages[PHARAOH_CITY_MESSAGE_RECORD_CAPACITY]);
void pharaoh_game_file_upgrade_message_text_ids(
    PharaohMenuMessageRecord messages[PHARAOH_CITY_MESSAGE_RECORD_CAPACITY]);
void pharaoh_city_message_initialize_problem_areas(
    PharaohCityMessageProblemState *state,
    PharaohCityMessageGetTime get_time, void *user_data);

#endif
