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
