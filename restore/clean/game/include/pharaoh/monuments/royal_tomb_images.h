#ifndef PHARAOH_MONUMENTS_ROYAL_TOMB_IMAGES_H
#define PHARAOH_MONUMENTS_ROYAL_TOMB_IMAGES_H
#include "pharaoh/monuments/image_updates.h"
#include "pharaoh/monuments/base_progress.h"

typedef struct PharaohRoyalTombImageContext {
    PharaohMapSurfaceContext *surface;
    const PharaohWalkerQueryRecord *walkers;
    const int32_t *last_part;
    const uint32_t *idle;
    const int32_t *stored_resources,*required_resources;
} PharaohRoyalTombImageContext;
enum PharaohRoyalTombImageClass {
    PHARAOH_TOMB_SMALL,PHARAOH_TOMB_MEDIUM,PHARAOH_TOMB_LARGE,PHARAOH_TOMB_GRAND
};
int32_t pharaoh_small_royal_tomb_chamber_count(void); /* 00545F60 */
int32_t pharaoh_medium_royal_tomb_chamber_count(void); /* 00559460 */
int32_t pharaoh_large_royal_tomb_chamber_count(void); /* 005567B0 */
int32_t pharaoh_grand_royal_tomb_chamber_count(void); /* 0054FED0 */
uint32_t pharaoh_small_royal_tomb_query_image(PharaohRoyalTombImageContext *c,
    int32_t building,uint32_t *flags); /* 00545760 */
uint32_t pharaoh_medium_royal_tomb_query_image(PharaohRoyalTombImageContext *c,
    int32_t building,uint32_t *flags); /* 00558C00 */
uint32_t pharaoh_large_royal_tomb_query_image(PharaohRoyalTombImageContext *c,
    int32_t building,uint32_t *flags); /* 00555EA0 */
uint32_t pharaoh_grand_royal_tomb_query_image(PharaohRoyalTombImageContext *c,
    int32_t building,uint32_t *flags); /* 0054F490 */
/* Bind query and actual workers-idle descendants, preserving other userdata.
 * Both image contexts reference the same live idle DWORD, last-part and scene. */
void pharaoh_royal_tomb_bind_image_query(PharaohMonumentImageContext *images,
    enum PharaohRoyalTombImageClass kind,PharaohRoyalTombImageContext *query);
/* Concrete small/medium/large/grand receivers, native rotation slots4..7. */
void pharaoh_rotated_bind_royal_tomb_images(PharaohRotatedBuildingOperations *ops,
    PharaohMonumentImageContext *const images[4],
    PharaohRoyalTombImageContext *const queries[4]);
#endif
