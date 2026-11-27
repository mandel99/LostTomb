#ifndef PHARAOH_BUILDING_CONSTRUCTION_PREVIEW_H
#define PHARAOH_BUILDING_CONSTRUCTION_PREVIEW_H
#include "pharaoh/building/construction_commit.h"
#include "pharaoh/map/construction_connections.h"

typedef struct PharaohConstructionPreviewCity {
    uint8_t native_0000[0x4600];
    uint32_t native_4600;
    uint8_t native_4604[0x3d4];
} PharaohConstructionPreviewCity;
_Static_assert(sizeof(PharaohConstructionPreviewCity)==0x49d8 &&
    offsetof(PharaohConstructionPreviewCity,native_4600)==0x4600,
    "native preview city view");

typedef struct PharaohConstructionPreviewContext {
    const PharaohConstructionCommitContext *commit;
    const PharaohConstructionConnectionContext *connection;
    const PharaohFerryRouteQueryContext *ferry_route;
    const PharaohFerryLandingMap *ferry_landings;
    const PharaohConstructionPreviewCity *cities; /* Same EA1870 city bank. */
    uint8_t *ferry_link_accepted; /* BCA315, same byte borrowed by commit. */
} PharaohConstructionPreviewContext;

/* Whole0046FEF0, reusing the actual MenuDragPreviewParent C algorithm.
 * All nested contexts alias the same live scene. Native logical return is
 * void; C status only propagates a genuine checked child IDIV fault. */
PharaohPercentageScaleStatus pharaoh_construction_preview(
    const PharaohConstructionPreviewContext *,int32_t x1,int32_t y1,
    int32_t x2,int32_t y2,int32_t tool);
#endif
