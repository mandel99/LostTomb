#include <string.h>
#include "pharaoh/monuments/stages.h"

enum {
    LIBRARY_PARTS = 25, LIBRARY_PREPARED = 2, LIBRARY_COMPLETE = 21,
    LIBRARY_WORK_TICKS = 100, CAESAREUM_PARTS = 9,
    CAESAREUM_COMPLETE = 19, ABU_SIMBEL_PARTS = 16,
    ABU_SIMBEL_COMPLETE = 56, LIGHTHOUSE_COMPLETE = 9,
    PART_KIND_WORKED = 0, PART_KIND_TOWER = 0, WORK_COMPLETE = 1,
    WALKER_LABORER = 85, ACTION_LABORER_IDLE = 8,
    KEPT_FROM_EARLIER_MISSION = 0xFFFF, SPOTS_PER_TABLE = 3,
    TOMB_NEIGHBOURS = 6, TOMB_OPENS_NEIGHBOURS = 3, TOMB_DONE = 4,
    TOMB_CHAMBER_SEALING = 17, TOMB_STOCK_FULL = 400,
    PROGRESS_COMPLETE = 100, SMALL_TOMB_PARTS = 167,
    MEDIUM_TOMB_PARTS = 158, LARGE_TOMB_PARTS = 437,
    GRAND_TOMB_PARTS = 513, TYPE_SMALL_TOMB = 229,
    TYPE_MEDIUM_TOMB = 234, TYPE_LARGE_TOMB = 235, TYPE_GRAND_TOMB = 236
};

static bool all_parts_prepared(const PharaohMonumentStageContext *c, int32_t main)
{
    int32_t id;
    for (id = main; id < main + LIBRARY_PARTS; ++id)
        if (c->buildings[id].stage < LIBRARY_PREPARED) return false;
    return true;
}

void pharaoh_caesareum_walker_work_tick(PharaohMonumentStageContext *c,
    void *object, int32_t building, int32_t walker, uint8_t done)
{
    (void)object;
    if (c->caesareum_part_kinds[c->buildings[building].subtype] == PART_KIND_WORKED)
        c->caesareum_spot_tick(c->user_data, building, walker, done);
}

void pharaoh_library_walker_work_tick(PharaohMonumentStageContext *c,
    void *object, int32_t building, int32_t walker, uint8_t done)
{
    PharaohMonumentBuilding *b = &c->buildings[building];
    PharaohMonumentWalker *w = &c->walkers[walker];
    int32_t main = building - b->subtype;
    int32_t id;
    if (all_parts_prepared(c, main)) {
        c->library_spot_tick(c->user_data, building, walker, done); return;
    }
    if (w->type != WALKER_LABORER || b->stage >= LIBRARY_PREPARED) {
        w->action_state = done; return;
    }
    if (++w->wait_ticks <= LIBRARY_WORK_TICKS) return;
    w->wait_ticks = 0; w->action_state = ACTION_LABORER_IDLE; b->stage++;
    c->update_image(c->user_data, object, building);
    main = building - b->subtype;
    if (!all_parts_prepared(c, main)) return;
    pharaoh_library_advance_stage(c, object, main);
    for (id = main; id < main + LIBRARY_PARTS; ++id) {
        int32_t x, y;
        c->get_work_tile(c->user_data, object, id, &x, &y);
        c->buildings[id].road_access_x = (int16_t)x;
        c->buildings[id].road_access_y = (int16_t)y;
        c->update_image(c->user_data, object, id);
    }
}

void pharaoh_lighthouse_walker_work_tick(PharaohMonumentStageContext *c,
    void *object, int32_t building, int32_t walker, uint8_t done)
{
    (void)object;
    if (c->lighthouse_part_kinds[c->buildings[building].subtype] == PART_KIND_TOWER)
        c->lighthouse_spot_tick(c->user_data, building, walker, done);
}

void pharaoh_library_set_completed(PharaohMonumentStageContext *c,
    void *object, int32_t building)
{
    int32_t id;
    for (id = building; id < building + LIBRARY_PARTS; ++id) {
        PharaohMonumentBuilding *part = &c->buildings[id];
        part->stage = LIBRARY_COMPLETE; part->is_main_part = 1;
        part->completion_shown = 1; part->created_sequence = KEPT_FROM_EARLIER_MISSION;
        c->update_image(c->user_data, object, id);
    }
}

void pharaoh_lighthouse_complete_instant(PharaohMonumentStageContext *c,
    void *object, int32_t building)
{
    PharaohMonumentBuilding *part = &c->buildings[building];
    part->stage = LIGHTHOUSE_COMPLETE; part->work_flags = WORK_COMPLETE;
    part->is_main_part = 3; part->completion_shown = 1;
    part->created_sequence = KEPT_FROM_EARLIER_MISSION;
    c->update_image(c->user_data, object, building);
}

void pharaoh_library_advance_stage(PharaohMonumentStageContext *c,
    void *object, int32_t building)
{
    int32_t main = c->main_part_id(c->user_data, building);
    int32_t stage = c->buildings[main].stage;
    int32_t id;
    if (stage >= LIBRARY_COMPLETE) return;
    for (id = main; id < main + LIBRARY_PARTS; ++id) {
        c->buildings[id].stage = stage + 1;
        c->update_image(c->user_data, object, id);
    }
}

void pharaoh_abu_simbel_advance_stage(PharaohMonumentStageContext *c,
    void *object, int32_t building)
{
    int32_t main = c->main_part_id(c->user_data, building);
    PharaohMonumentBuilding *b = &c->buildings[main];
    int32_t stage = b->city_slot, id, part = main;
    const uint8_t *entry;
    if (stage >= ABU_SIMBEL_COMPLETE) return;
    entry = c->abu_simbel_stage_table + stage * 4;
    if (entry[1] == 0) b->built_parts[entry[3] - 1] = entry[0] == 3;
    else {
        static const int8_t offsets[6] = { 0, 14, 11, 13, 15, 12 };
        if (entry[2] <= 5) part = main + offsets[entry[2]];
        c->buildings[part].stage = entry[3];
    }
    for (id = main; id < main + ABU_SIMBEL_PARTS; ++id) {
        c->buildings[id].city_slot = stage + 1;
        c->update_image(c->user_data, object, id);
    }
}

void pharaoh_caesareum_advance_stage(PharaohMonumentStageContext *c,
    void *object, int32_t building)
{
    int32_t main = c->main_part_id(c->user_data, building), i;
    PharaohMonumentBuilding *b = &c->buildings[main];
    if (b->stage >= CAESAREUM_COMPLETE) return;
    if (++b->stage == CAESAREUM_COMPLETE) b->work_flags |= WORK_COMPLETE;
    for (i = 0; i < CAESAREUM_PARTS; ++i)
        c->update_image(c->user_data, object, main + i);
}

void pharaoh_lighthouse_advance_stage(PharaohMonumentStageContext *c,
    void *object, int32_t building)
{
    PharaohMonumentBuilding *b = &c->buildings[building];
    if (b->stage >= LIGHTHOUSE_COMPLETE) return;
    if (++b->stage == LIGHTHOUSE_COMPLETE) b->work_flags |= WORK_COMPLETE;
    c->update_image(c->user_data, object, building);
}

static void tomb_advance_for_type(PharaohMonumentStageContext *c, int32_t building)
{
    PharaohMonumentBuilding *b = &c->buildings[building];
    if (b->type == TYPE_SMALL_TOMB)
        pharaoh_small_royal_tomb_part_advance(c, c->small_tomb_object, building);
    else if (b->type == TYPE_MEDIUM_TOMB)
        pharaoh_medium_royal_tomb_part_advance(c, c->medium_tomb_object, building);
    else if (b->type == TYPE_LARGE_TOMB)
        pharaoh_large_royal_tomb_part_advance(c, c->large_tomb_object, building);
    else if (b->type == TYPE_GRAND_TOMB)
        pharaoh_grand_royal_tomb_part_advance(c, c->grand_tomb_object, building);
}

void pharaoh_royal_tomb_part_check_stage_done(
    PharaohMonumentStageContext *c, int32_t building)
{
    PharaohMonumentBuilding *b = &c->buildings[building];
    int32_t table = c->tomb_spot_table(c->user_data, b->tomb_chamber_kind), s;
    if (table >= 0) for (s = 0; s < SPOTS_PER_TABLE; ++s)
        if (c->tomb_spot_has_work(c->user_data,
            &c->tomb_work_spots[table * SPOTS_PER_TABLE + s], b, b->tomb_stage)) return;
    tomb_advance_for_type(c, building);
}

static void tomb_work_tick(PharaohMonumentStageContext *c, int32_t table,
    int32_t building, int32_t walker, uint8_t next)
{
    PharaohMonumentWalker *w = &c->walkers[walker];
    PharaohMonumentBuilding *b = &c->buildings[building];
    PharaohTombWorkSpot *spots = &c->tomb_work_spots[table * SPOTS_PER_TABLE];
    PharaohTombWorkSpot *entry = NULL;
    int32_t s, stage = b->tomb_stage;
    w->is_ghost = 1;
    for (s = 0; s < SPOTS_PER_TABLE; ++s)
        if (spots[s].walker_id == walker) { entry = &spots[s]; break; }
    if (entry != NULL) {
        if (entry->stage != stage) { w->action_state = next; return; }
    } else {
        for (s = 0; s < SPOTS_PER_TABLE; ++s)
            if (spots[s].walker_id == 0 && spots[s].walker_type == w->type &&
                c->tomb_spot_has_work(c->user_data, &spots[s], b, stage)) {
                entry = &spots[s]; break;
            }
        if (entry == NULL) { w->action_state = next; return; }
    }
    if (entry->walker_id == 0) {
        entry->walker_id = walker; c->start_spot_animation(c->user_data, entry);
    }
    b->tomb_remaining_work[stage - 1]--;
    if (b->tomb_remaining_work[stage - 1] >= 1) return;
    b->tomb_worker_id = 0;
    pharaoh_royal_tomb_part_check_stage_done(c, building);
    w->action_state = next;
}

void pharaoh_royal_tomb_walker_work_tick(PharaohMonumentStageContext *c,
    void *object, int32_t building, int32_t walker, uint8_t next)
{
    int32_t table;
    (void)object;
    table = c->tomb_spot_table(c->user_data, c->buildings[building].tomb_chamber_kind);
    if (table >= 0) tomb_work_tick(c, table, building, walker, next);
}

void pharaoh_royal_tomb_part_advance_stage(PharaohMonumentStageContext *c,
    void *object, int32_t building, int32_t part_count)
{
    PharaohMonumentBuilding *b = &c->buildings[building];
    int32_t main = building - b->subtype, i;
    if (b->tomb_stage < TOMB_DONE) {
        b->tomb_stage++;
        if (b->tomb_stage == TOMB_OPENS_NEIGHBOURS) {
            for (i = 0; i < TOMB_NEIGHBOURS; ++i) {
                int32_t offset = b->tomb_neighbour_offsets[i];
                if (offset > -1) {
                    c->buildings[main + offset].tomb_stage++;
                    c->buildings[main + offset].tomb_can_take_workers = 1;
                }
            }
        }
        if (b->tomb_stage == TOMB_DONE) b->tomb_can_take_workers = 0;
        for (i = 0; i < part_count; ++i)
            c->update_image(c->user_data, object, main + i);
    }
    if (b->tomb_can_take_workers != 0u ||
        !c->victory_goals_reached(c->user_data)) return;
    main = building - b->subtype;
    if (c->tomb_progress(c->user_data, main, c->buildings[main].type) ==
        PROGRESS_COMPLETE) {
        c->buildings[main].tomb_completed = 1;
        c->update_tomb_final_image(c->user_data, main, c->buildings[main].type);
    }
}

void pharaoh_royal_tomb_set_completed(PharaohMonumentStageContext *c,
    void *object, int32_t building)
{
    int32_t main = c->main_part_id(c->user_data, building);
    int32_t end = main + c->object_part_count(c->user_data, object), id;
    c->buildings[main].tomb_stock = TOMB_STOCK_FULL;
    c->buildings[main].tomb_completed = 1;
    for (id = main; id < end; ++id) {
        PharaohMonumentBuilding *b = &c->buildings[id];
        if (b->tomb_chamber_kind != TOMB_CHAMBER_SEALING) b->tomb_stage = TOMB_DONE;
        b->created_sequence = KEPT_FROM_EARLIER_MISSION;
        b->completion_shown = 1;
    }
    for (id = main; id < end; ++id) c->update_image(c->user_data, object, id);
}

void pharaoh_royal_tomb_update_part_images(PharaohMonumentStageContext *c,
    void *object, int32_t building)
{
    int32_t main = c->main_part_id(c->user_data, building);
    int32_t end = main + c->object_chamber_count(c->user_data, object), id;
    for (id = main; id < end; ++id) c->update_image(c->user_data, object, id);
}

void pharaoh_small_royal_tomb_part_advance(PharaohMonumentStageContext *c,
    void *object, int32_t building)
{ pharaoh_royal_tomb_part_advance_stage(c, object, building, SMALL_TOMB_PARTS); }
void pharaoh_medium_royal_tomb_part_advance(PharaohMonumentStageContext *c,
    void *object, int32_t building)
{ pharaoh_royal_tomb_part_advance_stage(c, object, building, MEDIUM_TOMB_PARTS); }
void pharaoh_large_royal_tomb_part_advance(PharaohMonumentStageContext *c,
    void *object, int32_t building)
{ pharaoh_royal_tomb_part_advance_stage(c, object, building, LARGE_TOMB_PARTS); }
void pharaoh_grand_royal_tomb_part_advance(PharaohMonumentStageContext *c,
    void *object, int32_t building)
{ pharaoh_royal_tomb_part_advance_stage(c, object, building, GRAND_TOMB_PARTS); }

/* Completed prior C, native 00547BA0. */
int32_t pharaoh_royal_tomb_is_idle(
    void *object,
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t building_id, PharaohRoyalTombChamberCount chamber_count,
    PharaohRoyalTombHasWorker has_worker)
{
    const int32_t main_id = building_id - buildings[building_id].subtype;
    int32_t count;
    int32_t index;
    if (buildings[main_id].royal_tomb_stock > 0) return 0;
    count = chamber_count(object);
    for (index = 0; index < count; ++index) {
        if (has_worker(object, building_id + index) != 0) return 0;
    }
    return 1;
}

/* Completed prior C, native 004EA930. */
uint8_t pharaoh_monument_resources_complete(
    const int32_t *stored_amounts, const int32_t *required_amounts)
{
    static const uint8_t resources[15] = {
        1, 8, 10, 13, 15, 17, 18, 19, 20, 23, 24, 25, 26, 28, 30
    };
    uint32_t index;
    for (index = 0; index < 15u; ++index) {
        const uint32_t resource = resources[index];
        if (stored_amounts[resource] < required_amounts[resource]) return 0;
    }
    return 1;
}

/* Completed prior C, native 00545FB0. */
uint8_t pharaoh_monument_resources_complete_thunk(
    const int32_t *stored_amounts, const int32_t *required_amounts)
{
    return pharaoh_monument_resources_complete(
        stored_amounts, required_amounts);
}

void pharaoh_city_set_monument_building_id(
    int32_t *monument_building_ids, int32_t building_id, int32_t slot)
{
    monument_building_ids[slot] = building_id;
}

int32_t pharaoh_city_find_monument_slot(
    const int32_t monument_building_ids[3], int32_t building_id)
{
    for (int32_t slot = 0; slot < 3; ++slot) {
        if (monument_building_ids[slot] == building_id) return slot;
    }
    return -1;
}

uint8_t pharaoh_monument_object_has_incomplete(
    void *object,
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t building_type, int32_t building_count,
    PharaohMonumentExistingProgress progress)
{
    int32_t building_id;
    for (building_id = 1; building_id < building_count; ++building_id) {
        const PharaohBuildingTypeRecord *building = &buildings[building_id];
        if (building->state == 1 && building->building_type == building_type)
            return progress(object, building_id, 0) < 100;
    }
    return 0;
}

void pharaoh_monument_part_coordinates(
    const PharaohBuildingTypeRecord *building, int32_t step,
    int32_t pyramid_style, int32_t tile_pixel_width,
    int32_t tile_pixel_height, int32_t view_orientation,
    const PharaohMonumentCoordinateTables *tables, int32_t *x, int32_t *y)
{
    const uint8_t *record = (const uint8_t *)building;
    int32_t tier, height, index, orientation, rotation, slot;
    if (step < 1) {
        *x = 0;
        *y = 15;
        return;
    }
    tier = (step - 1) / 6;
    height = pyramid_style == 1 ?
        tables->style_one_heights[tier + building->pyramid_size_variant * 6] :
        tier * 90;
    *x = tile_pixel_width + 1;
    *y = -(tile_pixel_height + height);

    if (pyramid_style == 5) {
        index = building->subtype + building->pyramid_size_variant * 36;
        orientation = tables->style_five_orientations[index * 2];
    } else {
        index = building->subtype + building->pyramid_size_variant * 144;
        orientation = tables->regular_orientations[index * 2];
    }
    rotation = view_orientation / 2;
    if (orientation < 4) {
        orientation += rotation;
        if (orientation >= 4) orientation -= 4;
    } else if (orientation < 8) {
        orientation += rotation;
        if (orientation >= 8) orientation -= 4;
    }
    if (orientation < 9) {
        if (record[0x57] < record[0x58]) orientation = 8;
        slot = (step - 1) % 6 + orientation * 6;
        if (pyramid_style == 3) {
            index = slot * 4;
            if (building->origin_x == 0) {
                *x += tables->style_three_offsets[index];
                height = tables->style_three_offsets[index + 1];
            } else {
                *x += tables->style_three_offsets[index + 2];
                height = tables->style_three_offsets[index + 3];
            }
            *y += height;
            *x += 5;
            *y += 8;
            *x += 1;
            *y += 19;
            return;
        }
        index = slot * 2;
        *x += tables->regular_offsets[index];
        *y += tables->regular_offsets[index + 1];
        if (building->origin_x != 0) {
            *x += tables->main_part_offsets[orientation * 2];
            *y += tables->main_part_offsets[orientation * 2 + 1];
        }
    }
    *x += 1;
    *y += 19;
}

int32_t pharaoh_library_spot_table_variant(int32_t animation_counter)
{
    return (animation_counter / 2) & 1;
}

void pharaoh_library_spot_table_set_variant(int32_t building_variant,
    int32_t *active_variant, uint32_t destination[888],
    const uint32_t standard_source[888],
    const uint32_t alternate_source[888])
{
    if (building_variant != 0 && building_variant != 4) {
        *active_variant = 2;
        memcpy(destination, alternate_source, 888 * sizeof(uint32_t));
    } else {
        *active_variant = 0;
        memcpy(destination, standard_source, 888 * sizeof(uint32_t));
    }
}

static uint8_t monument_has_free_spot_for_type(
    const PharaohMonumentWorkAssignment *assignments, int32_t count,
    int32_t current_monument_id, int32_t worker_type)
{
    int32_t index;
    for (index = 0; index < count; ++index) {
        const PharaohMonumentWorkAssignment *a = &assignments[index];
        if (*(const int16_t *)(a->building + 0x12) == 0 &&
            a->monument_id == current_monument_id &&
            a->worker_type == worker_type &&
            *(const int32_t *)(a->building + 0x0c) > 0) return 1;
    }
    return 0;
}

static int32_t monument_find_work_for_walker(
    const PharaohMonumentWorkAssignment *assignments, int32_t count,
    const PharaohBuildingTypeRecord *buildings,
    const PharaohWalkerQueryRecord *walkers, int32_t building_id,
    int32_t walker_id)
{
    const int32_t phase = buildings[building_id].monument_stage;
    for (int32_t index = 0; index < count; ++index) {
        const PharaohMonumentWorkAssignment *a = &assignments[index];
        const uint8_t *spot = a->building;
        if (*(const int16_t *)(spot + 0x12) != 0 ||
            a->monument_id != phase) continue;
        /* Native reads the actor BYTE only after both descriptor gates. */
        const uint8_t worker_type =
            *(const volatile uint8_t *)&walkers[walker_id].type;
        if (a->worker_type != worker_type) continue;
        if (*(const int32_t *)(spot + 0x0c) > 0) return building_id;
    }
    return 0;
}

uint8_t pharaoh_monument_entrance_offset_6_minus_8(
    const PharaohMonumentEntranceBuilding *buildings, int32_t building_id,
    int32_t *x, int32_t *y)
{
    const PharaohMonumentEntranceBuilding *root =
        &buildings[building_id - buildings[building_id].main_part_delta];
    *x = root->x + 6;
    *y = root->y - 8;
    return 1;
}

uint8_t pharaoh_monument_menu_option_allowed(
    uint32_t phase, PharaohMonumentMenuOption option)
{
    const uint16_t kind = (uint16_t)option.words[0];
    const uint16_t subtype = (uint16_t)(option.words[0] >> 16);
    if (phase == 0u) return 0;
    if (kind == 2u &&
        (subtype == 1u || subtype == 4u || subtype == 2u || subtype == 3u))
        return 1;
    if (phase == 3u)
        return kind != 1u &&
            (subtype == 6u || subtype == 7u || subtype == 5u);
    if (phase == 2u)
        return kind != 1u &&
            (subtype == 10u || subtype == 11u ||
             subtype == 12u || subtype == 13u);
    if (phase == 4u) return kind != 1u && subtype == 14u;
    if (phase == 1u && kind != 1u) return 0;
    return 1;
}

uint8_t pharaoh_library_work_tile(
    const PharaohMonumentEntranceBuilding *buildings,
    const PharaohLibraryEntranceDefinition *definitions,
    int32_t building_id, int32_t *x, int32_t *y)
{
    const PharaohMonumentEntranceBuilding *building = &buildings[building_id];
    if (building->stage < 3) {
        const int32_t half =
            definitions[building->main_part_delta].width / 2;
        *x = building->x + half;
        *y = building->y + half;
    } else {
        building = &buildings[building_id - building->main_part_delta];
        *x = building->x + 3;
        *y = building->y + 11;
    }
    return 1;
}

uint8_t pharaoh_caesareum_work_tile(
    const PharaohMonumentEntranceBuilding *buildings,
    int32_t map_view_orientation, int32_t building_id,
    int32_t *x, int32_t *y)
{
    const PharaohMonumentEntranceBuilding *root =
        &buildings[building_id - buildings[building_id].main_part_delta];
    switch (map_view_orientation / 2) {
    case 0: *x = root->x + 8;  *y = root->y + 15; break;
    case 1: *x = root->x - 1;  *y = root->y + 8;  break;
    case 2: *x = root->x + 6;  *y = root->y - 1;  break;
    case 3: *x = root->x + 15; *y = root->y + 6;  break;
    }
    return 1;
}

void pharaoh_monument_spot_attach_figure(
    PharaohMonumentSpotAssignment *spot,
    const PharaohWalkerQueryRecord *walkers, int32_t walker_id)
{
    spot->walker_id = (int16_t)walker_id;
    if (walker_id != 0)
        spot->action_state = (uint8_t)walkers[walker_id].action_state;
}

int32_t pharaoh_sun_temple_find_work_for_walker(
    const PharaohMonumentWorkAssignment assignments[13],
    const PharaohBuildingTypeRecord *buildings,
    const PharaohWalkerQueryRecord *walkers, int32_t building_id,
    int32_t walker_id)
{
    return monument_find_work_for_walker(
        assignments, 13, buildings, walkers, building_id, walker_id);
}

void pharaoh_sphinx_count_assigned(
    const PharaohSphinxAssignment assignments[22], int32_t worker_type,
    int32_t *count)
{
    int32_t index;
    *count = 0;
    for (index = 0; index < 22; ++index)
        if (*(const int16_t *)(assignments[index].building + 0x12) != 0 &&
            assignments[index].worker_type == worker_type) ++*count;
}

uint8_t pharaoh_sphinx_has_free_spot_for_type(
    const PharaohSphinxAssignment assignments[22],
    int32_t current_monument_id, int32_t worker_type)
{
    int32_t index;
    for (index = 0; index < 22; ++index) {
        const PharaohSphinxAssignment *a = &assignments[index];
        if (*(const int16_t *)(a->building + 0x12) == 0 &&
            a->monument_id == current_monument_id &&
            a->worker_type == worker_type &&
            *(const int32_t *)(a->building + 0x0c) > 0) return 1;
    }
    return 0;
}

uint8_t pharaoh_sun_temple_has_free_spot_for_type(
    const PharaohMonumentWorkAssignment assignments[13],
    int32_t current_monument_id, int32_t worker_type)
{
    return monument_has_free_spot_for_type(
        assignments, 13, current_monument_id, worker_type);
}

int32_t pharaoh_library_find_work_for_walker(
    const PharaohMonumentWorkAssignment assignments[27],
    const PharaohBuildingTypeRecord *buildings,
    const PharaohWalkerQueryRecord *walkers, int32_t building_id,
    int32_t walker_id)
{
    return monument_find_work_for_walker(
        assignments, 27, buildings, walkers, building_id, walker_id);
}

uint8_t pharaoh_library_has_free_spot_for_type(
    const PharaohMonumentWorkAssignment assignments[27],
    int32_t current_monument_id, int32_t worker_type)
{
    return monument_has_free_spot_for_type(
        assignments, 27, current_monument_id, worker_type);
}

int32_t pharaoh_abu_simbel_find_work_for_walker(
    const PharaohMonumentWorkAssignment assignments[56],
    const PharaohBuildingTypeRecord *buildings,
    const PharaohWalkerQueryRecord *walkers, int32_t building_id,
    int32_t walker_id)
{
    return monument_find_work_for_walker(
        assignments, 56, buildings, walkers, building_id, walker_id);
}

uint8_t pharaoh_abu_simbel_has_free_spot_for_type(
    const PharaohMonumentWorkAssignment assignments[56],
    int32_t current_monument_id, int32_t worker_type)
{
    return monument_has_free_spot_for_type(
        assignments, 56, current_monument_id, worker_type);
}

int32_t pharaoh_caesareum_find_work_for_walker(
    const PharaohMonumentWorkAssignment assignments[27],
    const PharaohBuildingTypeRecord *buildings,
    const PharaohWalkerQueryRecord *walkers, int32_t building_id,
    int32_t walker_id)
{
    return monument_find_work_for_walker(
        assignments, 27, buildings, walkers, building_id, walker_id);
}

uint8_t pharaoh_caesareum_has_free_spot_for_type(
    const PharaohMonumentWorkAssignment assignments[27],
    int32_t current_monument_id, int32_t worker_type)
{
    return monument_has_free_spot_for_type(
        assignments, 27, current_monument_id, worker_type);
}

int32_t pharaoh_lighthouse_find_work_for_walker(
    const PharaohMonumentWorkAssignment assignments[15],
    const PharaohBuildingTypeRecord *buildings,
    const PharaohWalkerQueryRecord *walkers, int32_t building_id,
    int32_t walker_id)
{
    return monument_find_work_for_walker(
        assignments, 15, buildings, walkers, building_id, walker_id);
}

uint8_t pharaoh_lighthouse_has_free_spot_for_type(
    const PharaohMonumentWorkAssignment assignments[15],
    int32_t current_monument_id, int32_t worker_type)
{
    return monument_has_free_spot_for_type(
        assignments, 15, current_monument_id, worker_type);
}

void pharaoh_obelisk_count_assigned_for_type(int32_t walker_type,
    int32_t *count, int32_t building_type,
    const PharaohObeliskWorkSpot *small_spots,
    const PharaohObeliskWorkSpot *large_spots)
{
    const int32_t small_obelisk = 0xcf;
    const int32_t spot_count = building_type == small_obelisk ?
        10 : 18;
    const PharaohObeliskWorkSpot *spots =
        building_type == small_obelisk ? small_spots : large_spots;
    int32_t index;
    *count = 0;
    for (index = 0; index < spot_count; ++index) {
        if (spots[index].assignment->walker_id != 0 &&
            spots[index].walker_type == walker_type) {
            ++*count;
        }
    }
}

void pharaoh_obelisk_count_assigned_walkers(int32_t walker_type,
    int32_t *count,int32_t building_id,const int16_t *building_types,
    const PharaohObeliskWorkSpot *small_spots,const PharaohObeliskWorkSpot *large_spots)
{
    pharaoh_obelisk_count_assigned_for_type(walker_type,count,building_types[building_id],small_spots,large_spots);
}

uint8_t pharaoh_lighthouse_work_tile(
    const PharaohMonumentEntranceBuilding *buildings,
    int32_t building_id, int32_t *x, int32_t *y)
{
    *x = buildings[building_id].x + 2;
    *y = buildings[building_id].y + 6;
    return 1;
}

int32_t pharaoh_pyramid_next_work_piece(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t building_id, const PharaohPyramidWorkSegment segments[6])
{
    const int32_t part = buildings[building_id].subtype;
    const int32_t root = building_id - part;
    int32_t index;
    for (index = 0; index < 6; ++index) {
        const PharaohPyramidWorkSegment *segment = &segments[index];
        if (part < segment->low || part > segment->high) continue;
        if (part >= segment->low && part <= segment->descending_end) {
            if (part == segment->low)
                return root + segment->descending_end + 1;
            return building_id - 1;
        }
        if (part >= segment->ascending_start && part <= segment->high) {
            if (part == segment->high)
                return root + segment->ascending_start - 1;
            return building_id + 1;
        }
        return building_id + ((part & 1) != 0 ? -2 : 2);
    }
    return building_id;
}

int32_t pharaoh_monument_find_layout_pair(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t building_id, uint32_t pyramid_style,
    const PharaohMonumentLayoutPair *regular_pairs,
    const PharaohMonumentLayoutPair *style_five_pairs)
{
    const PharaohBuildingTypeRecord *building = &buildings[building_id];
    const uint32_t variant = building->pyramid_size_variant;
    const int32_t part = building->subtype;
    const int32_t root = building_id - part;
    const PharaohMonumentLayoutPair *pair = pyramid_style == 5u ?
        &style_five_pairs[variant * 20u] :
        &regular_pairs[variant * 78u];
    while (pair->left_part >= 0) {
        if (pair->right_part == part) return root + pair->left_part;
        if (pair->left_part == part) return root + pair->right_part;
        ++pair;
    }
    return -1;
}

int32_t pharaoh_pyramid_find_paired_work_piece(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t building_id, uint32_t pyramid_style,
    const PharaohMonumentLayoutPair *regular_pairs,
    const PharaohMonumentLayoutPair *style_five_pairs,
    const int32_t *frontier_parts, int32_t frontier_count,
    uint8_t *second_side)
{
    const PharaohBuildingTypeRecord *building = &buildings[building_id];
    const uint32_t variant = building->pyramid_size_variant;
    const int32_t part = building->subtype;
    const PharaohMonumentLayoutPair *pair = pyramid_style == 5u ?
        &style_five_pairs[variant * 20u] :
        &regular_pairs[variant * 78u];
    int32_t target = 0;
    uint8_t side = 0;
    int32_t index;
    *second_side = 0;
    while (pair->left_part >= 0) {
        if (part == pair->right_part) {
            target = pair->left_part;
            side = 1;
            break;
        }
        if (part == pair->left_part) {
            target = pair->right_part;
            side = 0;
            break;
        }
        ++pair;
    }
    if (pair->left_part < 0) return -1;
    for (index = 0; index < frontier_count; ++index) {
        const int32_t candidate = frontier_parts[index];
        if (buildings[candidate].subtype == target) {
            *second_side = side;
            return candidate;
        }
    }
    return -1;
}

int32_t pharaoh_pyramid_level_stage_next(int32_t stage)
{
    ++stage;
    return stage == 3 ? 4 : stage;
}

void pharaoh_city_clear_monument_building_id(
    int32_t *monument_building_ids, int32_t ignored_building_id, int32_t slot)
{
    (void)ignored_building_id;
    monument_building_ids[slot] = 0;
}

uint8_t pharaoh_monument_resource_unavailable(
    const int32_t *available_amounts, const int32_t *blocked_resources,
    int32_t resource, int32_t required_amount)
{
    if (required_amount <= available_amounts[resource] &&
        (required_amount < 1 || blocked_resources[resource] == 0)) return 0;
    return 1;
}
