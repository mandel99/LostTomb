#ifndef PHARAOH_MONUMENTS_PLACEMENT_H
#define PHARAOH_MONUMENTS_PLACEMENT_H
#include "pharaoh/monuments/image_updates.h"
#include "pharaoh/monuments/stages.h"

struct PharaohMonumentPlacementObject;
typedef uint8_t (*PharaohMonumentPlacementPredicate)(
    const struct PharaohMonumentPlacementObject *self,uint32_t terrain,int32_t part);
/* Logical views of actual native object members, not a replacement ABI frame.
 * parts points at the live self+offset listed beside each class entry.
 * special_part points at self+50 for a royal tomb. predicate is the real
 * native vtable+10 operation translated to one of the complete C leaves. */
typedef struct PharaohMonumentPlacementObject {
    const PharaohMonumentImagePart *parts;
    const uint32_t *special_part;
    PharaohMonumentPlacementPredicate predicate;
} PharaohMonumentPlacementObject;

typedef struct PharaohMonumentPlacementContext {
    const PharaohMapGridGeometry *geometry;
    const uint32_t *terrain;
    const int32_t *origin, *world_x, *world_y; /* BCA2D8/E8/EC */
    uint32_t *current_tile; /* BC9768 */
    const PharaohMapTileAreaOffset (*footprints)[6]; /* 5CC8C0, stride8 */
} PharaohMonumentPlacementContext;

void pharaoh_monument_placement_bind_surface(PharaohMonumentPlacementContext *c,
    const PharaohMapSurfaceContext *surface,const int32_t *world_x,const int32_t *world_y);

/* Public results expose logical AL. Known native callers TEST AL; unobserved
 * upper EAX/flags and unresolved indirect consumers are not this API. */
uint8_t pharaoh_monument_tile_is_free(const PharaohMonumentPlacementObject *self,
    uint32_t terrain,int32_t part);
uint8_t pharaoh_abu_simbel_tile_is_free(const PharaohMonumentPlacementObject *self,
    uint32_t terrain,int32_t part);
uint8_t pharaoh_royal_tomb_terrain_allowed(const PharaohMonumentPlacementObject *self,
    uint32_t terrain,int32_t part);
uint8_t pharaoh_lighthouse_tile_is_free(const PharaohMonumentPlacementObject *self,
    uint32_t terrain,int32_t part);
uint8_t pharaoh_monument_placement_check(const PharaohMonumentPlacementContext *c,
    const PharaohMonumentPlacementObject *self,uint32_t cell,
    const PharaohMonumentImagePart *pieces,int32_t count);
uint8_t pharaoh_royal_tomb_placement_check(const PharaohMonumentPlacementContext *c,
    const PharaohMonumentPlacementObject *self,uint32_t cell,
    const PharaohMonumentImagePart *pieces,int32_t count);

/* Native00401400: actual descriptors self+4C, 16 rows. */
uint8_t pharaoh_abu_simbel_can_place(const PharaohMonumentPlacementContext *c,
    const PharaohMonumentPlacementObject *self,uint32_t cell);
uint8_t pharaoh_abu_simbel_has_incomplete(void *object,
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    PharaohMonumentExistingProgress progress);

/* Native0040D850: actual descriptors self+4C, 25 rows. */
uint8_t pharaoh_library_can_place(const PharaohMonumentPlacementContext *c,
    const PharaohMonumentPlacementObject *self,uint32_t cell);
uint8_t pharaoh_library_has_incomplete(void *object,
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    PharaohMonumentExistingProgress progress);

/* Native00416F70: actual descriptors self+4C, 9 rows. */
uint8_t pharaoh_caesareum_can_place(const PharaohMonumentPlacementContext *c,
    const PharaohMonumentPlacementObject *self,uint32_t cell);
uint8_t pharaoh_caesareum_has_incomplete(void *object,
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    PharaohMonumentExistingProgress progress);

/* Native004E65F0: actual descriptors self+4C, 2 rows. */
uint8_t pharaoh_lighthouse_can_place(const PharaohMonumentPlacementContext *c,
    const PharaohMonumentPlacementObject *self,uint32_t cell);
uint8_t pharaoh_lighthouse_has_incomplete(void *object,
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    PharaohMonumentExistingProgress progress);

/* Native005456C0: actual descriptors self+1E5C, 167 rows. */
uint8_t pharaoh_small_royal_tomb_can_place(const PharaohMonumentPlacementContext *c,
    const PharaohMonumentPlacementObject *self,uint32_t cell);
uint8_t pharaoh_small_royal_tomb_has_incomplete(void *object,
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    PharaohMonumentExistingProgress progress);

/* Native0054F3F0: actual descriptors self+5C88, 513 rows. */
uint8_t pharaoh_grand_royal_tomb_can_place(const PharaohMonumentPlacementContext *c,
    const PharaohMonumentPlacementObject *self,uint32_t cell);
uint8_t pharaoh_grand_royal_tomb_has_incomplete(void *object,
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    PharaohMonumentExistingProgress progress);

/* Native00555E00: actual descriptors self+4EE0, 437 rows. */
uint8_t pharaoh_large_royal_tomb_can_place(const PharaohMonumentPlacementContext *c,
    const PharaohMonumentPlacementObject *self,uint32_t cell);
uint8_t pharaoh_large_royal_tomb_has_incomplete(void *object,
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    PharaohMonumentExistingProgress progress);

/* Native00558B60: actual descriptors self+1CBC, 158 rows. */
uint8_t pharaoh_medium_royal_tomb_can_place(const PharaohMonumentPlacementContext *c,
    const PharaohMonumentPlacementObject *self,uint32_t cell);
uint8_t pharaoh_medium_royal_tomb_has_incomplete(void *object,
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    PharaohMonumentExistingProgress progress);

#endif
