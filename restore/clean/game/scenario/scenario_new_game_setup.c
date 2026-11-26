#include "pharaoh/scenario/scenario_new_game_setup.h"
#include "pharaoh/core/percentage_adjust.h"
#include "pharaoh/core/static_objects.h"
#include "pharaoh/core/memory_fill.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

static void store_u16(uint8_t *destination, uint32_t offset, uint16_t value)
{
    memcpy(destination + offset, &value, sizeof(value));
}

static void store_u32(uint8_t *destination, uint32_t offset, uint32_t value)
{
    memcpy(destination + offset, &value, sizeof(value));
}

/* Complete typed translation of Pharaoh.exe 0x004E9EE0. */
static void pharaoh_scenario_player_gods_init(uint8_t *player)
{
    uint32_t god;

    for (god = 0; god < 5; ++god) {
        player[0x4390 + god] = 50;
        player[0x439a + god] = 50;
        player[0x43a4 + god] = 0;
        player[0x4816 + god] = 0;
        player[0x43c2 + god] = 0;
        player[0x43cc + god] = 0;
        player[0x43d6 + god] = 0;
        store_u32(player, 0x43e0 + god * 4, 0);
    }

    store_u16(player, 0x4984, UINT16_MAX);
    store_u16(player, 0x4986, UINT16_MAX);
    store_u16(player, 0x4988, UINT16_MAX);
    store_u32(player, 0x45cc, UINT32_MAX);
    store_u32(player, 0x47b4, UINT32_MAX);
    store_u32(player, 0x45d8, 0);
}

/* Complete typed translation of Pharaoh.exe 0x004E7D10. */
void pharaoh_scenario_player_custom_init(
    PharaohScenarioPlayerInitContext *context)
{
    uint8_t *const player = context->player_records + 0x49d8;

    pharaoh_memory_fill_bytes((uint32_t *)(void *)context->player_records,
        0x93b0, 0);
    pharaoh_native_flag_from_byte_004e7e20(context->flag_00ea1820,
        player[3]);
    *context->word_00ea186c = 0;

    store_u32(player, 0x14, 50);
    store_u32(player, 0x18, 50);
    store_u32(player, 0x10, 60);
    store_u32(player, 0x2c0c, 30);
    store_u32(player, 0x2c08, 30);
    store_u32(player, 0x20, 3);
    store_u32(player, 0x08, 9);
    store_u32(player, 0x42c4, 1);
    store_u32(player, 0x42c8, 1);
    store_u32(player, 0x25bc, 0);
    store_u32(player, 0x25c0, 0);
    store_u32(player, 0x438c, 1);
    store_u32(player, 0x44f4, 1);
    store_u32(player, 0x4574, 0);
    store_u32(player, 0x4578, 0);
    store_u32(player, 0x457c, 0);
    pharaoh_scenario_player_gods_init(player);
}

static uint32_t terminated_size(const char *text)
{
    uint32_t size = 0;
    do {
        ++size;
    } while (text[size - 1] != '\0');
    return size;
}

/* Complete typed C translation of Pharaoh.exe 0x00451BE0.
 * Global dword widths and array bounds are taken from the native stores. */
void pharaoh_scenario_runtime_state_reset(
    const PharaohScenarioRuntimeStateResetContext *state)
{
    uint32_t index;

    (*state->words_78d7a0)[16] = 0; /* 0x0078d7e0 */
    (*state->words_78d7a0)[11] = 0; /* 0x0078d7cc: gameTimeTick */
    (*state->words_78d7a0)[10] = 0; /* 0x0078d7c8 */
    (*state->words_78d7a0)[9] = 0;  /* 0x0078d7c4: gameTimeDay */
    (*state->words_78d7a0)[8] = 0;  /* 0x0078d7c0: currentMonth */
    (*state->words_78d7a0)[7] = UINT32_C(0x832); /* currentYear */
    (*state->words_78d7a0)[4] = 0;  /* 0x0078d7b0: mapSweepCountdown */
    (*state->words_78d7a0)[2] = 0;  /* 0x0078d7a8 */
    (*state->words_78d7a0)[1] = 0;  /* 0x0078d7a4 */
    (*state->words_78d7a0)[0] = 0;  /* 0x0078d7a0 */
    (*state->words_78d5c0)[1] = 0;  /* 0x0078d5c4 */
    (*state->words_78d5c0)[0] = 0;  /* 0x0078d5c0 */
    *state->word_78d78c = 0;
    pharaoh_native_ten_dwords_clear_0046a080(
        &(*state->words_78d5c0)[2]);

    for (index = 0; index < 0x33; ++index)
        (*state->words_78d3ec)[index] = 0;
    for (index = 0; index < 0x33; ++index)
        (*state->words_78d4b8)[index] = 0;
}

/* Complete typed translation of Pharaoh.exe 0x00540A10. */
void pharaoh_scenario_new_game_setup(
    PharaohScenarioNewGameSetupContext *context)
{
    uint32_t index;
    uint32_t name_size;
    uint32_t name_offset;
    int32_t treasury;

    *context->victory_state = 0;
    *context->victory_forced = 0;
    *context->victory_cheated = 0;
    *context->editor_construction_tool = 0;
    pharaoh_scenario_player_custom_init(context->player_init);
    pharaoh_native_flag_zero_004e7e10(context->player_init->flag_00ea1820);

    *context->current_player = 1;
    *context->player_flag_3 = 1;
    *context->player_flag_2 = 1;
    for (index = 0; index < 16; ++index)
        context->reset_steps[index](context->user_data);
    pharaoh_scenario_runtime_state_reset(context->runtime_reset);

    if (*context->editor_mode == 1) {
        *context->current_climate = 0;
        for (index = 0; index < 5; ++index)
            context->editor_steps[index](context->user_data);
    }

    context->initialize_custom_scenario(context->user_data);
    *context->current_player = 1;
    name_size = terminated_size(context->current_player_name);
    *context->player_flag_3 = 1;
    name_offset = 0;
    while (name_size - name_offset >= 4) {
        const uint8_t *source = (const uint8_t *)(const void *)(
            context->current_player_name + name_offset);
        context->player_name[name_offset + 0] = (char)source[0];
        context->player_name[name_offset + 1] = (char)source[1];
        context->player_name[name_offset + 2] = (char)source[2];
        context->player_name[name_offset + 3] = (char)source[3];
        name_offset += 4;
    }
    treasury = *context->player_treasury;
    *context->player_flag_2 = 1;
    while (name_offset < name_size) {
        context->player_name[name_offset] =
            context->current_player_name[name_offset];
        ++name_offset;
    }
    *context->player_name_initialized = 1;
    *context->player_treasury = pharaoh_adjust_by_percentage(treasury,
        context->difficulty_percentages[*context->difficulty_index]);
}
