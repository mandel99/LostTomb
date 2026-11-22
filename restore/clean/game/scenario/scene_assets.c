#include "pharaoh/scenario/scene_assets.h"

#include <string.h>

static void load(const PharaohSceneMonumentAssets *c,
    const char *name, int32_t slot)
{
    (void)pharaoh_sprite_library_load_file(c->libraries, name, slot);
}

static void shared_libraries(const PharaohSceneMonumentAssets *c)
{
    load(c, "CaesareumDummy", 0x12);
    load(c, "LtHouseDummy", 0x13);
    load(c, "LibMainDummy", 0x14);
    load(c, "LibMainDummy", 0x15);
    load(c, "LibMainDummy", 0x16);
    load(c, "LibExtraDummy", 0x17);
    load(c, "MausCarryoverDummy", 0x18);
}

void pharaoh_scene_load_monument_libraries(const PharaohSceneMonumentAssets *c)
{
    const uint8_t phase = *c->animation_set;
    if (phase <= 1) {
        *c->animation_set = 1;
        const int32_t style = pharaoh_scenario_get_pyramid_style(
            pharaoh_monument_definitions, c->goal_definition_ids);
        *c->pyramid_style = style;
        const char *name;
        switch (style) {
        case 1: name = "BENT_PYRAMID"; break;
        case 2: name = "MUDBRICK_PYRAMID"; break;
        case 3: name = "STEPPED_PYRAMID"; break;
        case 5: name = "MASTABA"; break;
        default: name = "PYRAMID"; break;
        }
        load(c, name, 0x11);
        shared_libraries(c);
        load(c, "TombThutDummy", 0x19);
        load(c, "TombTutDummy", 0x1b);
        load(c, "TombSetiDummy", 0x1c);
        load(c, "TombRamDummy", 0x1d);
        load(c, "AbuSimbelDummy", 0x1a);
        return;
    }
    if (phase == 2) {
        load(c, "PyramidDummy", 0x11);
        shared_libraries(c);
        load(c, "AbuSimbelDummy", 0x1a);
        const int32_t mission = *c->mission_id;
        load(c, mission >= 39 && mission <= 41 ?
            "TombThutFin" : "TombThut", 0x19);
        load(c, *c->mission_id == 40 || *c->mission_id == 41 ?
            "TombTutFin" : "TombTut", 0x1b);
        load(c, *c->mission_id == 41 ? "TombSetiFin" : "TombSeti", 0x1c);
        load(c, "TombRam", 0x1d);
        return;
    }
    if (phase == 4) {
        load(c, "AbuSimbel", 0x1a);
        load(c, "PyramidDummy", 0x11);
        shared_libraries(c);
        load(c, "TombThutDummy", 0x19);
        load(c, "TombSetiDummy", 0x1c);
        load(c, "TombTutDummy", 0x1b);
        load(c, "TombRamDummy", 0x1d);
        return;
    }
    load(c, "PyramidDummy", 0x11);
    load(c, "TombThutDummy", 0x19);
    load(c, "TombTutDummy", 0x1b);
    load(c, "TombSetiDummy", 0x1c);
    load(c, "TombRamDummy", 0x1d);
    load(c, "Caesareum1", 0x12);
    load(c, "LtHouse1", 0x13);
    load(c, "LibMaina1", 0x14);
    load(c, "LibMainb1", 0x15);
    load(c, "LibMainc1", 0x16);
    load(c, "LibExtra", 0x17);
    load(c, "MausCarryover", 0x18);
    load(c, "AbuSimbelDummy", 0x1a);
}

void pharaoh_scene_load_extra_library(const PharaohSceneMonumentAssets *c)
{
    const int32_t kind = strcmp(c->scenario_name, "The Glory of Ptolemy") == 0 ?
        4 : pharaoh_scenario_get_extra_script_monument(
            pharaoh_monument_definitions, c->goal_definition_ids);
    *c->extra_script_monument = kind;
    if (kind == 2) { load(c, "OBELISK_EXTRA", 9); return; }
    if (kind == 3) { load(c, "SUN_TEMPLE_EXTRA", 9); return; }
    if (kind != 4) { load(c, "SPHINX_EXTRA", 9); return; }
    const int32_t size = pharaoh_scenario_get_mausoleum_size(
        pharaoh_monument_definitions, c->goal_definition_ids);
    *c->mausoleum_size = size;
    load(c, size == 1 ? "MAUSOLEUM1_EXTRA" :
        size == 2 ? "MAUSOLEUM2_EXTRA" : "MAUSOLEUM0_EXTRA", 9);
}

void pharaoh_scene_initialize_enemy_properties(const PharaohSceneEnemyAssets *c)
{
    static const char *const names[12] = {
        "HYKSOS", "SEAPEOPLE", "HITTITE", "MITANI", "KUSHITE", "LIBIAN",
        "NUBIAN", "CANAANITE", "ASSYRIAN", "ROMAN", "PHOENICIAN", "PERSIAN"
    };
    static const uint32_t targets[5] = {44, 43, 45, 93, 92};
    int32_t selected = *c->enemy_type;
    if (selected >= 12) {
        selected = 11;
        *c->enemy_type = 11;
    }
    (void)pharaoh_sprite_library_load_file(c->libraries, names[selected], 2);
    /* Native retains selected across the load, then copies one DWORD into
     * each of five rows per iteration, in this exact interleaved order. */
    for (uint32_t field = 0; field < 14; ++field)
        for (uint32_t column = 0; column < 5; ++column)
            memcpy((uint8_t *)&(*c->figure_models)[targets[column]] + field * 4,
                (const uint8_t *)&(*c->enemy_models)[selected][column] + field * 4, 4);
}
