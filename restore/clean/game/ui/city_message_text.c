#include "pharaoh/ui/city_message_text.h"

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
