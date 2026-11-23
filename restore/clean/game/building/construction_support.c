#include "pharaoh/building/construction_support.h"
#include "pharaoh/building/construction_load.h"
#include "pharaoh/core/static_objects.h"
#include "pharaoh/monuments/stages.h"

#include <string.h>

_Static_assert(sizeof(PharaohBuildingStorageRecord)==196 &&
    offsetof(PharaohBuildingStorageRecord,in_use)==8 &&
    offsetof(PharaohBuildingStorageRecord,resource_state)==10 &&
    offsetof(PharaohBuildingStorageRecord,accept_amount)==0x34 &&
    offsetof(PharaohBuildingStorageRecord,get_amount)==0x7c &&
    sizeof(PharaohFerryQueueRecord)==224 &&
    offsetof(PharaohFerryQueueRecord,building_id)==220,
    "native construction storage and ferry records");
_Static_assert(offsetof(PharaohBuildingTypeRecord,runtime_value_038)==0x38 &&
    offsetof(PharaohBuildingTypeRecord,pyramid_work_phase)==0x55 &&
    offsetof(PharaohScenarioPlayerRuntime,monument_building_ids)==0x473c &&
    sizeof(PharaohScenarioPlayerRuntime)==0x49d8,
    "native construction monument fields and shared player stride");

/* Reused StorageSettings C, native0046B100. Availability is recomputed even
 * when every allocation slot is occupied. Slot0 is deliberately skipped. */
uint32_t pharaoh_construction_storage_allocate(
    PharaohConstructionSupportContext *c,int32_t building)
{
    pharaoh_city_resource_determine_available(c->resources);
    uint32_t slot;
    for(slot=1;slot<PHARAOH_BUILDING_STORAGE_COUNT;++slot)
        if(c->storages[slot].in_use==0) break;
    if(slot==PHARAOH_BUILDING_STORAGE_COUNT) return 0;
    PharaohBuildingStorageRecord *const storage=&c->storages[slot];
    memset(storage,0,sizeof(*storage));
    const int32_t maximum=*c->highest_storage;
    storage->in_use=1;
    storage->building_id=(int16_t)building;
    if(maximum<=(int32_t)slot) *c->highest_storage=(int32_t)slot;
    const int32_t kind=c->surface->buildings[building].building_type;
    if(kind==70||kind==190) {
        uint32_t i=0;
        if(*c->resources->resource_count>0) do {
            const uint32_t resource=(uint32_t)c->resources->resources[i];
            if(pharaoh_native_type_is_supported_005013a0(resource))
                storage->resource_state[resource]=0;
            ++i;
        } while((int32_t)i<*c->resources->resource_count);
        return slot;
    }
    if(kind!=71&&kind!=72) return slot;
    for(uint32_t i=0;i<36;++i) {
        storage->resource_state[i]=1;
        storage->accept_amount[i]=3200;
        storage->get_amount[i]=3200;
    }
    if(kind==71) {
        uint32_t remaining=(uint32_t)*c->resources->food_count,i=0;
        if((int32_t)remaining>0) do {
            const uint32_t resource=(uint32_t)c->resources->foods[i];
            ++i;
            storage->resource_state[resource]=0;
        } while(--remaining!=0);
    } else {
        uint32_t remaining=(uint32_t)*c->resources->resource_count,i=0;
        if((int32_t)remaining>0) do {
            const uint32_t resource=(uint32_t)c->resources->resources[i];
            if((int32_t)resource<1||(int32_t)resource>8)
                storage->resource_state[resource]=0;
            ++i;
        } while(--remaining!=0);
    }
    return slot;
}

/* Reused BuildingSupport C, native0046EFD0. Allocation zeroes the entire row;
 * ring initialization remains the caller's subsequent operation. */
uint32_t pharaoh_construction_ferry_queue_allocate(
    PharaohConstructionSupportContext *c,uint32_t building)
{
    for(uint32_t slot=1;slot<PHARAOH_FERRY_QUEUE_COUNT;++slot) {
        PharaohFerryQueueRecord *const queue=&c->ferry_queues[slot];
        if(queue->queue.state!=0) continue;
        memset(queue,0,sizeof(*queue));
        queue->queue.state=1;
        queue->building_id=building<=INT32_MAX?(int32_t)building:
            (int32_t)((int64_t)building-INT64_C(4294967296));
        return slot;
    }
    return 0;
}

/* Reused BuildingSupport C, native004F4F80. EAA978 is player1's definition
 * array (EA1870+49D8+4730), independent of the current player's building IDs. */
int32_t pharaoh_construction_monument_register(
    PharaohConstructionSupportContext *c,int32_t building)
{
    if(c->surface->buildings[building].runtime_value_038!=0) return -1;
    const int32_t definition=pharaoh_monument_definition_for_building(c->monuments,building);
    if(definition<0) return -1;
    for(int32_t slot=0;slot<3;++slot) {
        if(c->monuments->goal_definition_ids[slot]!=definition ||
            c->monuments->players[1].monument_definition_ids[slot]!=0) continue;
        c->monuments->players[1].monument_definition_ids[slot]=definition;
        pharaoh_city_set_monument_building_id(
            c->monuments->players[*c->monuments->current_player].monument_building_ids,
            building,slot);
        return slot;
    }
    return -1;
}

/* Native004E57F0 stores slot AX but returns only AL=1 after the reset child.
 * Do not expose its untouched upper EAX as a WORD or DWORD result. */
uint8_t pharaoh_construction_obelisk_create(
    PharaohConstructionSupportContext *c,int32_t building)
{
    (void)pharaoh_construction_monument_register(c,building);
    const int32_t slot=pharaoh_city_find_monument_slot(
        c->monuments->players[*c->monuments->current_player].monument_building_ids,
        building);
    c->surface->buildings[building].subtype=(int16_t)slot;
    c->surface->buildings[building].pyramid_work_phase=33;
    pharaoh_obelisk_reset_work_spots(c->surface->buildings[building].building_type,
        c->animations->small_obelisk,c->animations->large_obelisk,
        c->animation_scripts,c->animation_script_count,c->random);
    return UINT8_C(1);
}

/* Reused FarmDisplay54 C, native004E5840. The loader may mutate the building:
 * subtype and footprint are deliberately reloaded after the library call. */
void pharaoh_construction_obelisk_update_image(
    PharaohConstructionSupportContext *c,int32_t building,int32_t x,int32_t y)
{
    PharaohBuildingTypeRecord *const record=&c->surface->buildings[building];
    int32_t progress=((int32_t)record->pyramid_work_phase-66)*3;
    if(progress<0) progress=0;
    const bool small=record->building_type==207;
    const int32_t last=small?3:5;
    int32_t stage=(last+1)*progress/100;
    if(stage>last) stage=last;
    (void)pharaoh_sprite_library_load_file(c->libraries,
        c->obelisk_scripts[(small?0:4)+stage],record->subtype+5);
    const int32_t group=record->subtype==0?564:record->subtype==1?565:
        record->subtype==2?566:0;
    const uint32_t image=group?c->surface->groups[group].first_image:0;
    pharaoh_map_place_surface_square(c->surface,(uint16_t)building,x,y,
        record->size,record->size,image,8);
}

static uint8_t storage_adapter(void *data,int32_t building)
{return (uint8_t)pharaoh_construction_storage_allocate(data,building);}
static uint8_t ferry_adapter(void *data,uint32_t building)
{return (uint8_t)pharaoh_construction_ferry_queue_allocate(data,building);}
static void register_adapter(void *data,int32_t building)
{(void)pharaoh_construction_monument_register(data,building);}
static void obelisk_adapter(void *data,int32_t building)
{(void)pharaoh_construction_obelisk_create(data,building);}
static void obelisk_image_adapter(void *data,int32_t building,int32_t x,int32_t y)
{pharaoh_construction_obelisk_update_image(data,building,x,y);}

void pharaoh_construction_bind_support_operations(
    PharaohConstructionLoadOperations *ops,PharaohConstructionSupportContext *c)
{
    ops->support_user_data=c;
    ops->storage_allocate=storage_adapter;
    ops->ferry_queue_allocate=ferry_adapter;
    ops->sphinx_create=register_adapter;
    ops->obelisk_create=obelisk_adapter;
    ops->obelisk_update_image=obelisk_image_adapter;
}
