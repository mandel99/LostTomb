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
