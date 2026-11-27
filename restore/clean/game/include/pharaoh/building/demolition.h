#ifndef PHARAOH_BUILDING_DEMOLITION_H
#define PHARAOH_BUILDING_DEMOLITION_H

#include "pharaoh/building/construction_undo.h"
#include "pharaoh/core/ring_queue.h"
#include "pharaoh/map/bridge_removal.h"
#include "pharaoh/monuments/pyramid_demolition.h"
#include "pharaoh/population/displaced.h"
#include "pharaoh/runtime/city_warning.h"
#include "pharaoh/runtime/sound_effect_request.h"
#include "pharaoh/scenario/scene_routing.h"
#include "pharaoh/ui/window_state_actions.h"

/* Native BC9698..BC96B7. Coordinates retain the original request order. */
typedef struct PharaohDemolitionState {
    int32_t y2,x2,y1,x1;
    uint32_t classes,sensitive,line,pair;
} PharaohDemolitionState;
_Static_assert(sizeof(PharaohDemolitionState)==32 &&
    offsetof(PharaohDemolitionState,classes)==0x10 &&
    offsetof(PharaohDemolitionState,pair)==0x1c,"native demolition controls");

typedef struct PharaohDemolitionContext {
    PharaohMapSurfaceContext *surface;
    const PharaohConstructionUndoContext *backup;
    uint8_t *cleared; /* AE8E7C, also the live surface canal-water grid. */
    uint32_t *placed_count; /* BC971C. */
    PharaohDemolitionState *state;
    const PharaohBridgeRemovalContext *bridge;
    const PharaohPyramidDemolitionContext *pyramid;
    const PharaohDisplacedPopulationContext *displaced;
    const uint32_t *current_player;
    PharaohCityWarningContext *warnings;
    PharaohSoundEffectRequestState *sound_state;
    const PharaohDigitalSoundDeviceContext *sound_device;
    PharaohSceneRouting *routing;
    PharaohPopupWindowState *popup;
    uint8_t (*native_records_7900c0)[196]; /* Only BYTE0 is written here. */
    uint8_t (*native_records_9783a4)[28]; /* Only BYTE0 is written here. */
    PharaohFerryQueueRecord *ferry_queues; /* EAaf68, state DWORD0, stride224. */
} PharaohDemolitionContext;

/* Complete474700/4743F0. Context and backing state must persist until a
 * popup response. All contexts borrow the same world, undo, queues and
 * cursors; active ferries require the real routing providers. Adjacent venue
 * imaging retains its existing native producer domain: direction0/1 and
 * even view0..6; unsupported private stack-image history is unresolved. */
void pharaoh_construction_demolish(const PharaohDemolitionContext *,
    int32_t preview,int32_t x1,int32_t y1,int32_t x2,int32_t y2);
void pharaoh_construction_request_demolition(const PharaohDemolitionContext *,
    int32_t preview,int32_t x1,int32_t y1,int32_t x2,int32_t y2);

/* Whole fixed Native callbacks4021D0/2210/2250/2290, logical void. */
void pharaoh_demolition_confirm_monument(const PharaohDemolitionContext *);
void pharaoh_demolition_confirm_class(const PharaohDemolitionContext *);
void pharaoh_demolition_confirm_pair(const PharaohDemolitionContext *);
void pharaoh_demolition_confirm_bridge(const PharaohDemolitionContext *);

#endif
