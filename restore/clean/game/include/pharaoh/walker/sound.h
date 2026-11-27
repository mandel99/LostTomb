#ifndef PHARAOH_WALKER_SOUND_H
#define PHARAOH_WALKER_SOUND_H

#include "pharaoh/walker/record_queries.h"
#include "pharaoh/runtime/city_view.h"
#include "pharaoh/runtime/digital_sound_device.h"

typedef struct PharaohWalkerSoundContext {
    const PharaohWalkerQueryRecord *walkers;
    const PharaohCityViewState *city_view;
    const PharaohCityProjectionLookup *projection;
    const PharaohCityMapBounds *map_bounds;
    const volatile int32_t *enabled;             /* F55B88 */
    const volatile uint8_t *effects_enabled;     /* E38E63 */
    const volatile int32_t *fullscreen;          /* F53F78 */
    const volatile int32_t *fullscreen_width;    /* F53F60 */
    const volatile int32_t *window_width;        /* F53F64 */
    const volatile int32_t *screen_height;       /* E38D98 */
    const volatile int32_t *volume;              /* E38EC4 */
    const int32_t *sample_ids;                   /* F54120 */
    PharaohDigitalSoundRequest *request;         /* E92DA8 */
    PharaohDigitalSoundDeviceState *device;      /* E92BF8 */
    const PharaohDigitalSoundDeviceOperations *operations;
} PharaohWalkerSoundContext;

/* Complete0053B4A0 and004B5210. Shared cells and real sound operations required. */
void pharaoh_walker_sound_play(const PharaohWalkerSoundContext *context,
    int32_t walker_id,int32_t sound,int32_t variant);
void pharaoh_walker_combat_sound(const PharaohWalkerSoundContext *context,
    int32_t walker_id);

#endif
