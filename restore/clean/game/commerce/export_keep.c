#include "pharaoh/commerce/export_keep.h"
#include "pharaoh/ui/economy_actions.h"

static int workshop_type_uses_resource_model(int32_t type)
{
    if (type >= 110 && type <= 114) return 1;
    switch (type) {
    case 32: case 47: case 51: case 53: case 74: case 177: case 178:
    case 187: case 188: case 189: case 194: case 203: case 204: case 205:
    case 226: case 231: case 232: case 233:
        return 1;
    default:
        return 0;
    }
}

int32_t pharaoh_building_count_workshops_using_resource(
    const PharaohBuildingTypeRecord *buildings, int32_t highest_id,
    uint32_t resource)
{
    static const int8_t resource_models[33] = {
        21, -1, -1, -1, -1, -1, 22, 13, 8, 5, 7, 24, 9, 19, 1, 16,
        10, -1, 4, 15, 11, 12, 3, 6, -1, -1, 14, 17, 18, 23, 20, 25, 26
    };
    const int16_t model = resource - 2u < 33u ?
        resource_models[resource - 2u] : -1;
    int32_t total = 0;

    for (int32_t id = 1; id <= highest_id; ++id) {
        const PharaohBuildingTypeRecord *const building = &buildings[id];
        if (building->state != 1 || building->num_workers == 0 ||
            !workshop_type_uses_resource_model(building->building_type)) {
            continue;
        }
        if (building->subtype == model ||
            building->secondary_resource_model == model) {
            ++total;
        }
    }
    return total;
}

int32_t pharaoh_city_resource_cap_amount(uint32_t resource, int32_t value)
{
    int32_t limit;
    if (value < 0) return 0;
    switch (resource) {
    case 10: case 28:
        limit = 10000;
        break;
    case 12: case 24: case 25: case 26: case 30:
        limit = 30000;
        break;
    default:
        limit = 15000;
        break;
    }
    return value > limit ? limit : value;
}

PharaohScenarioEvent *pharaoh_scenario_request_next_active(
    PharaohScenarioEvent *events, int32_t count,
    const PharaohScenarioEvent *previous)
{
    int32_t index = previous != NULL ? previous->id + 1 : 0;
    for (; index < count; ++index) {
        PharaohScenarioEvent *event = &events[index];
        if (event->type == 1u && event->is_active != 0u) {
            event->id = (int16_t)index;
            return event;
        }
    }
    return NULL;
}

uint8_t pharaoh_event_record_is_enabled_0044d4c0(const uint8_t *record)
{
    const int16_t state = *(const int16_t *)(record + 0x30);
    return record[2] == 1 && record[0x32] != 0 &&
        (state == 0 || state == 1);
}

/* Literal planned-kind/resource costs from complete LateMenuActions C. */
static uint32_t planned_cost(int32_t kind, int32_t resource)
{
    switch (kind) {
    case 1:case 13:return resource==24?4800u:resource==25?19200u:0u;
    case 2:case 14:return resource==24?21600u:resource==25?43200u:0u;
    case 3:return resource==12?4800u:resource==25?19200u:0u;
    case 4:return resource==12?21600u:resource==25?43200u:0u;
    case 5:return resource==12?67200u:resource==25?76800u:0u;
    case 6:return resource==12?144000u:resource==25?120000u:0u;
    case 7:return resource==12?264000u:resource==25?160000u:0u;
    case 8:return resource==24?4800u:0u;
    case 9:return resource==24?21600u:0u;
    case 10:return resource==24?67200u:0u;
    case 11:return resource==24?144000u:0u;
    case 12:return resource==24?264000u:0u;
    case 15:return resource==24?67200u:resource==25?76800u:0u;
    case 16:return resource==24?144000u:resource==25?120000u:0u;
    case 17:return resource==24?264000u:resource==25?160000u:0u;
    case 18:return resource==12?10800u:0u;
    case 19:return resource==12?24000u:0u;
    case 20:return resource==12?42000u:0u;
    case 22:return resource==26?10000u:0u;
    case 23:return resource==26?20000u:0u;
    case 24:return resource==30?38000u:0u;
    case 25:case 26:case 27:return resource==30?49600u:0u;
    default:return 0;
    }
}

/* Complete typed reuse, native004F57B0. Reload current player per slot. */
int32_t pharaoh_monument_resources_needed(PharaohMonumentResourceDemand *c,
    int32_t resource)
{
    uint32_t total=0;
    for (uint32_t slot=0;slot<3;++slot) {
        const int32_t player=*c->current_player;
        const int32_t id=c->players[player].monument_building_ids[slot];
        PharaohBuildingTypeRecord *b=&c->complex->buildings[id];
        if (id && b->state==1) {
            if (b->building_type==0xde) {
                int32_t out[12];
                (void)pharaoh_mausoleum_progress(c->complex,id,out);
                if (resource==30) total+=(uint32_t)out[7];
            } else if (b->building_type==0xd5) {
                int32_t out[14];
                (void)pharaoh_sun_temple_progress(c->complex,id,out);
                if (resource==30) total+=(uint32_t)out[11];
            } else if (b->building_type==0xb7) {
                PharaohPyramidProgressReport out;
                uint32_t first,second;
                pharaoh_pyramid_build_progress_report(c->pyramid,id,&out);
                first=(uint32_t)out.mainRemaining;
                second=(uint32_t)out.casingRemaining;
                switch (*c->pyramid->style) {
                case 1:case 4:
                    if (resource==25) {if ((int32_t)second>0) total+=second*400u;}
                    else if (resource==24 && (int32_t)first>0) total+=first*400u;
                    break;
                case 2:
                    if (resource==25) {if ((int32_t)second>0) total+=second*400u;}
                    else if (resource==12 && (int32_t)first>0) total+=first*400u;
                    break;
                case 3:
                    if (resource==24 && ((int32_t)first>0 || (int32_t)second>0))
                        total+=(first+second)*400u;
                    break;
                case 5:
                    if (resource==12 && ((int32_t)first>0 || (int32_t)second>0))
                        total+=(first+second)*400u;
                    break;
                }
            }
        } else total+=planned_cost(c->planned_kinds[slot],resource);
    }
    return (int32_t)total;
}

/* Complete Unit00618 C reuse, native00443330. Request and stock costs wrap
 * at32 bits before the native signed cap; resource0 is not written. */
void pharaoh_empire_update_export_keep(PharaohExportKeepContext *c)
{
    uint32_t costs[36]={0};
    PharaohScenarioEvent *event=pharaoh_scenario_request_next_active(
        c->events,*c->event_count,NULL);
    while (event) {
        if (pharaoh_event_record_is_enabled_0044d4c0((const uint8_t *)event)) {
            const int32_t kind=event->item.value;
            if (kind>0 && kind<36)
                costs[kind]+=(uint32_t)(int32_t)event->amount.value*100u;
        }
        event=pharaoh_scenario_request_next_active(c->events,*c->event_count,event);
    }
    for (uint32_t resource=1;resource<36;++resource) {
        uint32_t base=1000,j;
        costs[resource]+=(c->produced[resource]-c->consumed[resource])*100u;
        for (j=0;j<6;++j) {
            const uint32_t category=c->categories[j];
            if ((category==1 && resource<=8) || category==resource) {
                const int32_t population=c->initialize->players[
                    *c->initialize->current_player].population;
                const uint32_t band=population<1000?0:population<2500?1:population<5000?2:3;
                base=c->population_limits[band][j];
                break;
            }
        }
        if (j==6) {
            switch (resource) {
            case 9:case 11:case 14:case 16:case 18:case 20:case 22:case 29:case 32:
                base=((uint32_t)pharaoh_building_count_workshops_using_resource(
                    c->monuments->complex->buildings,*c->highest_building,resource)+1u)*200u;
                break;
            case 12:case 24:case 25:case 26:case 30:
                base=(uint32_t)pharaoh_monument_resources_needed(c->monuments,(int32_t)resource)+500u;
                break;
            }
        }
        c->export_keep[resource]=pharaoh_city_resource_cap_amount(resource,
            (int32_t)(base+costs[resource]));
    }
}

static void export_keep(void *user_data)
{
    pharaoh_empire_update_export_keep(user_data);
}

void pharaoh_saved_game_bind_export_keep(
    PharaohSavedGameInitializeOperations *ops, PharaohExportKeepContext *c)
{
    c->initialize->operations=ops;
    c->monuments->players=c->initialize->players;
    c->monuments->current_player=c->initialize->current_player;
    c->monuments->pyramid->buildings=c->monuments->complex->buildings;
    c->monuments->pyramid->walkers=c->monuments->complex->walkers;
    ops->step[PHARAOH_INIT_EXPORT_KEEP]=(PharaohSavedGameStepBinding){c,export_keep};
}
