#ifndef PHARAOH_RUNTIME_CAMERA_SCROLL_H
#define PHARAOH_RUNTIME_CAMERA_SCROLL_H

#include "pharaoh/runtime/game_tick_gate.h"
#include "pharaoh/runtime/city_view.h"

typedef struct PharaohCameraScrollInput {
    const int32_t *cursor_x, *cursor_y, *screen_width, *screen_height;
    uint32_t *requested, *west, *east, *north, *south;
    uint32_t *last_ready_tick;
    const int32_t *scroll_speed;
    const PharaohGameTickGateOperations *clock;
} PharaohCameraScrollInput;

typedef struct PharaohCameraScrollContext {
    const int32_t *large_vertical_step; /* native5E2A60 !=0 selects4, else1 */
    PharaohCityViewContext *view;
    /* Native702984:69 strided field views, not the70-record initializer
       base702900. Only DWORD fields0 and2..6 of each128-byte stride are used. */
    int32_t (*decay_records)[32];
} PharaohCameraScrollContext;

/* Complete native004CF660. Full result0..8; a closed cadence gate consumes
   no key flags and leaves requested untouched. Key slots are distinct DWORDs. */
int32_t pharaoh_camera_scroll_direction(const PharaohCameraScrollInput *);
/* Complete native004DDDE0. Direction8 returns; other unrecognized values
   still decay records and clamp. Coordinate arithmetic wraps at32 bits. */
void pharaoh_camera_scroll_apply(const PharaohCameraScrollContext *,int32_t direction);

#endif
