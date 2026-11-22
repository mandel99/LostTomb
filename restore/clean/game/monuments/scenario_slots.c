#include "pharaoh/monuments/scenario_slots.h"
#include "pharaoh/scenario/definition_counts.h"

/* Native initialized tables0056F7D8/0056F7EC. */
uint32_t pharaoh_pyramid_part_counts[5] = {16, 36, 64, 100, 144};
uint32_t pharaoh_mastaba_part_counts[3] = {10, 21, 36};

static int32_t find_option(const PharaohMonumentSlotContext *c,
    const PharaohBuildingTypeRecord *building, int32_t kind,
    int32_t subkind, int32_t size, int32_t mode)
{
    for (int32_t i = 0; i < pharaoh_monument_definition_count(); ++i) {
        const PharaohMonumentDefinition *d = &pharaoh_monument_definitions[i];
        if (d->kind != kind) continue;
        if (mode == 0) {
            const int32_t family = pharaoh_scenario_get_pyramid_style(
                pharaoh_monument_definitions, c->goal_definition_ids);
            if (d->subkind == family && (uint16_t)d->size == building->pyramid_size_variant)
                return i;
        } else if (d->subkind == subkind) {
            if (mode == 3) return i;
            if (mode == 2) size = pharaoh_scenario_get_mausoleum_size(
                pharaoh_monument_definitions, c->goal_definition_ids);
            if (d->size == size) return i;
        }
    }
    return -1;
}

/* Complete MapCommitSelection C, native004F4B70. */
int32_t pharaoh_monument_definition_for_building(
    const PharaohMonumentSlotContext *c, int32_t building_id)
{
    const PharaohBuildingTypeRecord *b = &c->buildings[building_id];
    switch (b->building_type) {
    case 0xb7: return find_option(c, b, 1, 0, 0, 0);
    case 0xcf: return find_option(c, b, 2, 2, 0, 1);
    case 0xd0: return find_option(c, b, 2, 2, 1, 1);
    case 0xd2: return find_option(c, b, 2, 1, 0, 3);
    case 0xd5: return find_option(c, b, 2, 3, 0, 3);
    case 0xde: {
        const int32_t result = find_option(c, b, 2, 4, 0, 2);
        if (result != -1) return result;
        return find_option(c, b, 2, 10, 0, 3);
    }
    case 0xe1: return find_option(c, b, 2, 6, 0, 3);
    case 0xe3: return find_option(c, b, 2, 7, 0, 3);
    case 0xe4: return find_option(c, b, 2, 5, 0, 3);
    case 0xe5: return find_option(c, b, 2, 10, 0, 3);
    case 0xe6: return find_option(c, b, 2, 14, 0, 3);
    case 0xea: return find_option(c, b, 2, 11, 0, 3);
    case 0xeb: return find_option(c, b, 2, 12, 0, 3);
    case 0xec: return find_option(c, b, 2, 13, 0, 3);
    default: return -1;
    }
}

/* Complete MapCommitSelection C, native004F61E0. */
void pharaoh_monument_rebuild_player_slots(const PharaohMonumentSlotContext *c)
{
    PharaohScenarioPlayerRuntime *p = &c->players[*c->current_player];
    for (int32_t slot = 0; slot < 3; ++slot) {
        p->monument_definition_ids[slot] = 0;
        p->monument_building_ids[slot] = 0;
    }
    for (uint32_t id = 1; (int32_t)id <= *c->highest_building; ++id) {
        const uint32_t original = id;
        const PharaohBuildingTypeRecord *b = &c->buildings[id];
        if ((b->state != 1 && b->state != 3) || b->runtime_value_016 == -1) continue;
        int32_t result;
        switch (b->building_type) {
        case 0xb7:
            result = pharaoh_monument_definition_for_building(c, (int32_t)id);
            id += (*c->pyramid_style == 5 ? c->mastaba_parts : c->pyramid_parts)
                [b->pyramid_size_variant] - 1u;
            break;
        case 0xcf: case 0xd0: case 0xe4:
            result = pharaoh_monument_definition_for_building(c, (int32_t)id); break;
        case 0xd2:
            if (b->runtime_value_038 != 0) continue;
            result = pharaoh_monument_definition_for_building(c, (int32_t)id); break;
        case 0xd5: result = pharaoh_monument_definition_for_building(c, (int32_t)id); id += 99; break;
        case 0xde: result = pharaoh_monument_definition_for_building(c, (int32_t)id); id += 115; break;
        case 0xe1: result = pharaoh_monument_definition_for_building(c, (int32_t)id); id += 24; break;
        case 0xe3: result = pharaoh_monument_definition_for_building(c, (int32_t)id); id += 8; break;
        case 0xe5: result = pharaoh_monument_definition_for_building(c, (int32_t)id); id += 166; break;
        case 0xe6: result = pharaoh_monument_definition_for_building(c, (int32_t)id); id += 15; break;
        case 0xea: result = pharaoh_monument_definition_for_building(c, (int32_t)id); id += 157; break;
        case 0xeb: result = pharaoh_monument_definition_for_building(c, (int32_t)id); id += 436; break;
        case 0xec: result = pharaoh_monument_definition_for_building(c, (int32_t)id); id += 512; break;
        default: continue;
        }
        if (result > 0) {
            p = &c->players[*c->current_player];
            for (int32_t slot = 0; slot < 3; ++slot) {
                if (c->goal_definition_ids[slot] == result && p->monument_building_ids[slot] == 0) {
                    p->monument_definition_ids[slot] = result;
                    p->monument_building_ids[slot] = (int32_t)original;
                    break;
                }
            }
        }
    }
}
