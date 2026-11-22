#include "pharaoh/monuments/definitions.h"

/* Native initialized table005D19C8; all38 sixteen-byte records retained. */
PharaohMonumentDefinition pharaoh_monument_definitions[38] = {
    {0,0,0,{0,0,0},{0,0,0,0}},
    {1,1,0,{24,25,20},{4,0,1,0}},
    {1,1,1,{24,25,20},{10,0,1,0}},
    {1,2,0,{12,25,20},{4,0,1,0}},
    {1,2,1,{12,25,20},{12,0,1,0}},
    {1,2,2,{12,25,20},{26,0,1,0}},
    {1,2,3,{12,25,20},{48,0,1,0}},
    {1,2,4,{12,25,20},{80,0,1,0}},
    {1,3,0,{24,0,20},{3,0,1,0}},
    {1,3,1,{24,0,20},{9,0,1,0}},
    {1,3,2,{24,0,20},{20,0,1,0}},
    {1,3,3,{24,0,20},{36,0,1,0}},
    {1,3,4,{24,0,20},{60,0,1,0}},
    {1,4,0,{24,25,20},{4,0,1,0}},
    {1,4,1,{24,25,20},{12,0,1,0}},
    {1,4,2,{24,25,20},{26,0,1,0}},
    {1,4,3,{24,25,20},{48,0,1,0}},
    {1,4,4,{24,25,20},{80,0,1,0}},
    {1,5,0,{12,0,0},{2,0,1,0}},
    {1,5,1,{12,0,0},{3,0,1,0}},
    {1,5,2,{12,0,0},{11,0,1,0}},
    {2,1,0,{24,20,0},{10,0,0,0}},
    {2,2,0,{26,20,0},{1,0,0,0}},
    {2,2,1,{26,20,0},{3,0,0,0}},
    {2,3,0,{30,20,0},{4,0,1,0}},
    {2,4,0,{30,20,0},{5,0,1,0}},
    {2,4,1,{30,20,0},{5,0,1,0}},
    {2,4,2,{30,20,0},{5,0,1,0}},
    {2,5,0,{35,20,0},{8,0,1,0}},
    {2,6,0,{35,29,20},{12,0,1,0}},
    {2,7,0,{35,26,20},{10,0,1,0}},
    {2,8,0,{30,25,20},{5,0,1,0}},
    {2,9,0,{30,25,20},{5,0,1,0}},
    {2,10,0,{33,34,11},{4,0,1,0}},
    {2,11,1,{33,34,11},{12,0,1,0}},
    {2,12,2,{33,34,11},{26,0,1,0}},
    {2,13,3,{33,34,11},{48,0,1,0}},
    {2,14,0,{20,30,0},{48,0,1,0}}
};


/* Complete logical-byte C translation of Pharaoh.exe 0x004F56A0. */
uint8_t pharaoh_monument_definition_uses_resource(
    const PharaohMonumentDefinition *definitions,
    int32_t definition, int32_t resource)
{
    const PharaohMonumentDefinition *entry = &definitions[definition];
    return entry->resources[0] == resource ||
        entry->resources[1] == resource || entry->resources[2] == resource;
}

/* Complete C translation of Pharaoh.exe 0x004F52B0. */
int32_t pharaoh_scenario_monument_slot_obelisk_type(
    const PharaohMonumentDefinition *definitions,
    const int16_t *goal_definition_ids, int32_t slot)
{
    const PharaohMonumentDefinition *entry =
        &definitions[goal_definition_ids[slot]];
    if (entry->kind == 2 && entry->subkind == 2)
        return 0xcf + (entry->size != 0);
    return 0;
}

/* Complete logical-byte C translation of Pharaoh.exe 0x004F52F0. */
uint8_t pharaoh_scenario_monument_slot_is_pyramid(
    const PharaohMonumentDefinition *definitions,
    const int16_t *goal_definition_ids, int32_t slot)
{
    const PharaohMonumentDefinition *entry =
        &definitions[goal_definition_ids[slot]];
    return (entry->kind == 1 && entry->subkind >= 1 &&
            entry->subkind <= 5) || entry->subkind == 4;
}

/* Complete C translation of Pharaoh.exe 0x004F44E0. */
int32_t pharaoh_scenario_get_pyramid_style(
    const PharaohMonumentDefinition *definitions,
    const int16_t goal_definition_ids[3])
{
    for (int32_t slot = 0; slot < 3; ++slot) {
        const int32_t id = goal_definition_ids[slot];
        if (id != 0 && definitions[id].kind == 1)
            return definitions[id].subkind;
    }
    return 4;
}

/* Complete C translation of Pharaoh.exe 0x0053D8E0. */
int32_t pharaoh_scenario_get_extra_script_monument(
    const PharaohMonumentDefinition *definitions,
    const int16_t goal_definition_ids[2])
{
    for (int32_t slot = 0; slot < 2; ++slot) {
        const int32_t id = goal_definition_ids[slot];
        if (id != 0 && definitions[id].kind == 2 &&
            definitions[id].subkind <= 4)
            return definitions[id].subkind;
    }
    return 1;
}

/* Complete C translation of Pharaoh.exe 0x0053D930. */
int32_t pharaoh_scenario_get_mausoleum_size(
    const PharaohMonumentDefinition *definitions,
    const int16_t goal_definition_ids[2])
{
    for (int32_t slot = 0; slot < 2; ++slot) {
        const int32_t id = goal_definition_ids[slot];
        if (id != 0 && definitions[id].kind == 2 &&
            definitions[id].subkind == 4)
            return definitions[id].size;
    }
    return 0;
}
