#ifndef PHARAOH_BUILDING_CONSTRUCTION_SUPPORT_H
#define PHARAOH_BUILDING_CONSTRUCTION_SUPPORT_H

#include "pharaoh/commerce/resource_supply.h"
#include "pharaoh/commerce/storage_queries.h"
#include "pharaoh/core/ring_queue.h"
#include "pharaoh/map/surface_tiles.h"
#include "pharaoh/monuments/scenario_slots.h"
#include "pharaoh/rendering/monument_spot_animation.h"

typedef struct PharaohConstructionSupportContext {
    PharaohMapSurfaceContext *surface;
    PharaohBuildingStorageRecord *storages; /*200 native196-byte rows*/
    int32_t *highest_storage;
    const PharaohResourceAvailabilityContext *resources;
    PharaohFerryQueueRecord *ferry_queues; /*50 native224-byte rows; slot0 reserved*/
    const PharaohMonumentSlotContext *monuments;
    const PharaohMonumentAnimationRegistry *animations;
    const PharaohMonumentAnimScript *animation_scripts;
    int32_t animation_script_count;
    PharaohCrtRandom *random;
    const PharaohSpriteLibraryLoadContext *libraries;
    const char *const *obelisk_scripts; /*ten native script names*/
} PharaohConstructionSupportContext;

/* Resource IDs are native indices0..35. The player view in monuments must be
 * the shared EA1870/49D8 backing store, not copied monument slot state. */
uint32_t pharaoh_construction_storage_allocate(
    PharaohConstructionSupportContext *c,int32_t building); /*0046B100; full EAX*/
uint32_t pharaoh_construction_ferry_queue_allocate(
    PharaohConstructionSupportContext *c,uint32_t building); /*0046EFD0; full EAX*/
int32_t pharaoh_construction_monument_register(
    PharaohConstructionSupportContext *c,int32_t building); /*004F4F80; -1 or slot0..2*/
uint8_t pharaoh_construction_obelisk_create(
    PharaohConstructionSupportContext *c,int32_t building); /*004E57F0; AL=1, upper EAX unspecified*/
void pharaoh_construction_obelisk_update_image(
    PharaohConstructionSupportContext *c,int32_t building,int32_t x,int32_t y); /*004E5840*/

struct PharaohConstructionLoadOperations;
/* Attach the five actual C providers; retain the other native children and
 * their common user_data. Both records must outlive construction calls. */
void pharaoh_construction_bind_support_operations(
    struct PharaohConstructionLoadOperations *operations,
    PharaohConstructionSupportContext *context);
#endif
