#ifndef PHARAOH_MONUMENTS_IMAGE_QUERIES_H
#define PHARAOH_MONUMENTS_IMAGE_QUERIES_H

#include "pharaoh/monuments/image_updates.h"
#include "pharaoh/assets/sprite_libraries.h"

/* Live scene views. Stage scripts are the native receiver's pointer table:
 * Caesareum +220, Lighthouse +B4. The pointed-to names remain caller-owned. */
typedef struct PharaohMonumentQueryContext {
    PharaohMapSurfaceContext *surface;
    const PharaohMonumentImagePart *parts;
    const PharaohSpriteLibraryLoadContext *libraries;
    const char *const *stage_scripts;
} PharaohMonumentQueryContext;

uint32_t pharaoh_abu_simbel_query_image(PharaohMonumentQueryContext *c,
    int32_t building,uint32_t *flags); /* 00401800 */
uint32_t pharaoh_library_query_image(PharaohMonumentQueryContext *c,
    int32_t building,uint32_t *flags); /* 0040DE80 */
uint32_t pharaoh_caesareum_query_image(PharaohMonumentQueryContext *c,
    int32_t building,uint32_t *flags); /* 00417310 */
uint32_t pharaoh_lighthouse_query_image(PharaohMonumentQueryContext *c,
    int32_t building,uint32_t *flags); /* 004E6A70 */
uint32_t pharaoh_sun_temple_query_image(PharaohMonumentQueryContext *c,
    int32_t building,uint32_t *flags); /* 005414B0 */

enum PharaohMonumentQueryMethod {
    PHARAOH_QUERY_ABU_SIMBEL, PHARAOH_QUERY_LIBRARY, PHARAOH_QUERY_CAESAREUM,
    PHARAOH_QUERY_LIGHTHOUSE, PHARAOH_QUERY_SUN_TEMPLE
};
/* Bind this query only; retain the image parent's other callback state. */
void pharaoh_monument_bind_image_query(PharaohMonumentImageContext *images,
    enum PharaohMonumentQueryMethod method,PharaohMonumentQueryContext *query);

#endif
