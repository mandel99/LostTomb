#include "pharaoh/scenario/campaign_records.h"

#include <limits.h>

typedef struct WinnerCandidate {
    int32_t value;
    uint32_t index;
    uint8_t tie_breaker;
} WinnerCandidate;

static void update_winner(WinnerCandidate *winner, int32_t value,
    uint8_t tie_breaker, uint32_t index)
{
    if (value > winner->value ||
        (value == winner->value && tie_breaker > winner->tie_breaker)) {
        winner->value = value;
        winner->tie_breaker = tie_breaker;
        winner->index = index;
    }
}

void pharaoh_mission_records_store(PharaohMissionRecordState *state,
    const PharaohMissionRecordOperations *operations)
{
    WinnerCandidate best[3] = {
        { -1, UINT32_MAX, 0 },
        { -1, UINT32_MAX, 0 },
        { -1, UINT32_MAX, 0 }
    };

    *state->stored_personal_savings = state->current_personal_savings;
    for (int32_t index = 1; index < state->monument_count; ++index) {
        const PharaohMissionMonumentRecord *const record =
            &state->monuments[index];
        if (record->active == 1 && record->eligible != 0) {
            const uint32_t slot = record->type == 12 ? 0u :
                record->type == 11 ? 1u : 2u;
            update_winner(&best[slot], record->value,
                record->tie_breaker, (uint32_t)index);
        }
    }
    for (uint32_t slot = 0; slot < 3; ++slot) {
        PharaohMissionWinner *const output = &state->winners[slot];
        output->value = best[slot].value;
        output->tie_breaker = best[slot].tie_breaker;
        output->result_id = best[slot].index == UINT32_MAX ? UINT8_MAX :
            state->monuments[best[slot].index].result_id;
    }

    const char *const text = operations->select_text(operations->user_data,
        0x133, (int32_t)state->settings->mission_id);
    if (text[6] == '1') {
        for (int32_t index = 1; index < state->building_count; ++index) {
            const PharaohMissionBuildingRecord *const building =
                &state->buildings[index];
            if (building->active != 1) continue;
            switch (building->type) {
            case 0xde:
                state->special_positions->type_de_first =
                    building->packed_first;
                state->special_positions->type_de_packed =
                    (uint16_t)(building->packed_low |
                        (uint16_t)((uint32_t)building->packed_high << 13));
                break;
            case 0xe1:
                state->special_positions->type_e1_x =
                    (uint16_t)(building->x - 4u);
                state->special_positions->type_e1_y =
                    (uint16_t)(building->y + 2u);
                break;
            case 0xe4: {
                const int32_t parent = index - building->parent_offset;
                state->special_positions->type_e4_x =
                    state->buildings[parent].x;
                state->special_positions->type_e4_y =
                    state->buildings[parent].y;
                break;
            }
            case 0xe5:
                state->special_positions->type_e5_x =
                    (uint16_t)(building->x - 5u);
                state->special_positions->type_e5_y =
                    (uint16_t)(building->y + 6u);
                break;
            case 0xea:
                state->special_positions->type_ea_x =
                    (uint16_t)(building->x - 6u);
                state->special_positions->type_ea_y =
                    (uint16_t)(building->y + 8u);
                break;
            case 0xeb:
                state->special_positions->type_eb_x =
                    (uint16_t)(building->x + 1u);
                state->special_positions->type_eb_y =
                    (uint16_t)(building->y + 17u);
                break;
            default:
                break;
            }
        }
    }
    operations->save_profile(operations->user_data);
}
