#include "pharaoh/ui/city_message_text.h"

#include <stddef.h>
#include <string.h>

/* Complete C translation of Pharaoh.exe 0x004E1600. */
int32_t pharaoh_city_message_text_id(int32_t message_id)
{
    return (int32_t)((uint32_t)message_id +
        (uint32_t)(message_id < 51 ? 99 : 199));
}

/* Complete C translation of Pharaoh.exe 0x004E14B0. */
void pharaoh_city_message_set_category(uint32_t message, uint32_t *category)
{
    switch (message) {
    case 0x71: case 0x76: case 0x77:
    case 0x7a: case 0x7b: case 0x7c: case 0x7d:
    case 0x7e: case 0x7f: case 0x80: case 0x81:
        *category = 5;
        break;
    case 0x82: case 0x135: case 0x13a:
        *category = 4;
        break;
    case 0x84:
        *category = 8;
        break;
    case 0x87: case 0x88:
        *category = 1;
        break;
    case 0x12d: case 0x12e: case 0x12f:
        *category = 6;
        break;
    default:
        *category = 0;
        break;
    }
}

/* Complete C translation of Pharaoh.exe 0x004E1750. */
int32_t pharaoh_city_message_new_id(
    const PharaohMenuMessageRecord messages[PHARAOH_CITY_MESSAGE_RECORD_CAPACITY])
{
    int32_t index;
    for (index = 0; index < PHARAOH_CITY_MESSAGE_RECORD_CAPACITY; ++index) {
        if (messages[index].message == 0) return index;
    }
    return -1;
}

void pharaoh_game_file_upgrade_message_text_ids(
    PharaohMenuMessageRecord messages[PHARAOH_CITY_MESSAGE_RECORD_CAPACITY])
{
    int32_t index;
    for (index = 0; index < PHARAOH_CITY_MESSAGE_RECORD_CAPACITY; ++index) {
        const int16_t message = messages[index].message;
        if (message == 0) return;
        messages[index].message =
            (int16_t)pharaoh_city_message_text_id((int32_t)message);
    }
}

/* Complete typed translation of Pharaoh.exe 0x004E1830. */
void pharaoh_city_message_initialize_problem_areas(
    PharaohCityMessageProblemState *state,
    PharaohCityMessageGetTime get_time, void *user_data)
{
    state->problem_area_b = 0;
    state->problem_count = 0;
    state->last_update_time = get_time(user_data) - 10000u;
}

_Static_assert(sizeof(PharaohMenuMessageRecord) == 48 &&
    offsetof(PharaohMenuMessageRecord, message) == 0x0a &&
    offsetof(PharaohMenuMessageRecord, template_id) == 0x10,
    "native city-message reset fields");

/* Complete typed reuse of MissionRecordActions.c, qualified against the
 * native stores at004E1770. The child clock call occurs before the final
 * two transient counters are cleared. Other message bytes remain intact. */
void pharaoh_city_message_initialize_scenario(
    const PharaohCityMessageScenarioReset *s)
{
    for (int32_t i = 0; i < PHARAOH_CITY_MESSAGE_RECORD_CAPACITY; ++i) {
        s->messages[i].message = 0;
        s->messages[i].template_id = -1;
    }
    memset(s->population_shown, 0, 8);
    *s->messages_used = 0;
    memset(s->counts, 0, 20 * sizeof(*s->counts));
    memset(s->delays, 0, 20 * sizeof(*s->delays));
    memset(s->queue, 0, 20 * sizeof(*s->queue));
    s->population_shown[8] = 0;
    s->population_shown[9] = 0;
    *s->queue_delay = 0;
    *s->next_sequence = 0;
    *s->current_id = -1;
    *s->shown_index = 0;
    *s->scroll_position = 0;
    *s->list_scroll = 0;
    *s->posted_once = 1;
    *s->fire_last_time = 0;
    *s->collapse_last_time = 0;
    *s->reserved2 = 0;
    *s->reserved1 = 0;
    pharaoh_city_message_initialize_problem_areas(s->problem,
        s->get_time, s->clock_user_data);
    *s->count_problem = 0;
    *s->queue_reserved = 0;
}
