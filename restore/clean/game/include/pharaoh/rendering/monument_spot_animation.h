#ifndef PHARAOH_RENDERING_MONUMENT_SPOT_ANIMATION_H
#define PHARAOH_RENDERING_MONUMENT_SPOT_ANIMATION_H

#include <stdint.h>

#include "pharaoh/core/crt_random.h"
#include "pharaoh/walker/record_queries.h"

enum { PHARAOH_MONUMENT_ANIMATION_VIEWS = 4 };

typedef struct PharaohMonumentAnimPosition {
    int32_t x;
    int32_t y;
} PharaohMonumentAnimPosition;

typedef struct PharaohMonumentAnimImages {
    int32_t image_group;
    int32_t image_offset;
    int32_t frames;
    int32_t frame_step;
} PharaohMonumentAnimImages;

typedef struct PharaohMonumentAnimSequence {
    int32_t position_from;
    int32_t position_to;
    int32_t images;
    int32_t last_frame;
    int32_t image_add[PHARAOH_MONUMENT_ANIMATION_VIEWS];
    int32_t image_add_reverse[PHARAOH_MONUMENT_ANIMATION_VIEWS];
} PharaohMonumentAnimSequence;

typedef struct PharaohMonumentAnimTransition {
    int32_t sequence;
    uint8_t reverse;
    uint8_t reserved[3];
} PharaohMonumentAnimTransition;

typedef struct PharaohMonumentAnimScript {
    const PharaohMonumentAnimPosition *positions;
    int32_t position_count;
    const PharaohMonumentAnimImages *images;
    int32_t image_count;
    const PharaohMonumentAnimSequence *sequences;
    int32_t sequence_count;
    const void *transitions;
    int32_t transition_count;
    int32_t x_offset;
    int32_t y_offset;
} PharaohMonumentAnimScript;

typedef struct PharaohMonumentSpotAnimation {
    int32_t script;
    int32_t sequence;
    int32_t frame;
    int32_t work_remaining;
    int16_t mode;
    int16_t walker_id;
    uint8_t walker_action;
    uint8_t reverse;
    uint8_t reserved[2];
} PharaohMonumentSpotAnimation;

typedef struct PharaohMonumentWorkSpotBinding {
    PharaohMonumentSpotAnimation *spot;
    int32_t initial_work_remaining;
    uint32_t reserved[2];
} PharaohMonumentWorkSpotBinding;

typedef struct PharaohMonumentWorkSpotGroup {
    PharaohMonumentWorkSpotBinding *bindings;
    const int32_t *initial_work_override;
} PharaohMonumentWorkSpotGroup;

typedef struct PharaohMonumentSpotDrawContext {
    const PharaohMonumentAnimScript *scripts;
    int32_t script_count;
    const int32_t *image_group_first;
    const int16_t *sprite_offset_x;
    const int16_t *sprite_offset_y;
    int32_t image_record_count;
    int32_t map_orientation;
} PharaohMonumentSpotDrawContext;

typedef struct PharaohMonumentAnimationRegistry {
    PharaohMonumentWorkSpotBinding *sphinx;
    PharaohMonumentWorkSpotBinding *sun_temple;
    PharaohMonumentWorkSpotBinding *library;
    PharaohMonumentWorkSpotBinding *abu_simbel;
    PharaohMonumentWorkSpotBinding *caesareum;
    PharaohMonumentWorkSpotBinding *lighthouse;
    PharaohMonumentWorkSpotGroup *royal_tomb;
    PharaohMonumentWorkSpotBinding *small_obelisk;
    PharaohMonumentWorkSpotBinding *large_obelisk;
    void *user_data;
    int32_t (*find_incomplete_obelisk)(void *user_data);
    int16_t (*building_type)(void *user_data, int32_t building_id);
} PharaohMonumentAnimationRegistry;

void pharaoh_monument_spot_get_draw_info(
    const PharaohMonumentSpotDrawContext *context,
    const PharaohMonumentSpotAnimation *spot,
    int32_t *x, int32_t *y, int32_t *image);
void pharaoh_monument_spot_get_draw_info_for_direction(
    const PharaohMonumentSpotDrawContext *context,
    const PharaohMonumentSpotAnimation *spot,
    int32_t *x, int32_t *y, int32_t *image, int32_t direction);
void pharaoh_monument_spot_start(
    PharaohMonumentSpotAnimation *spot, int16_t mode,
    const PharaohMonumentAnimScript *scripts, int32_t script_count,
    PharaohCrtRandom *random);
void pharaoh_sphinx_reset_work_spots(PharaohMonumentWorkSpotBinding *bindings,
    const PharaohMonumentAnimScript *scripts, int32_t script_count,
    PharaohCrtRandom *random);
void pharaoh_sun_temple_reset_work_spots(PharaohMonumentWorkSpotBinding *bindings,
    const PharaohMonumentAnimScript *scripts, int32_t script_count,
    PharaohCrtRandom *random);
void pharaoh_library_reset_work_spots(PharaohMonumentWorkSpotBinding *bindings,
    const PharaohMonumentAnimScript *scripts, int32_t script_count,
    PharaohCrtRandom *random);
void pharaoh_abu_simbel_reset_work_spots(PharaohMonumentWorkSpotBinding *bindings,
    const PharaohMonumentAnimScript *scripts, int32_t script_count,
    PharaohCrtRandom *random);
void pharaoh_caesareum_reset_work_spots(PharaohMonumentWorkSpotBinding *bindings,
    const PharaohMonumentAnimScript *scripts, int32_t script_count,
    PharaohCrtRandom *random);
void pharaoh_lighthouse_reset_work_spots(PharaohMonumentWorkSpotBinding *bindings,
    const PharaohMonumentAnimScript *scripts, int32_t script_count,
    PharaohCrtRandom *random);
void pharaoh_royal_tomb_reset_work_spots(
    const PharaohMonumentWorkSpotGroup groups[26],
    const PharaohMonumentAnimScript *scripts, int32_t script_count,
    PharaohCrtRandom *random);
void pharaoh_obelisk_reset_work_spots(int16_t building_type,
    PharaohMonumentWorkSpotBinding small_bindings[10],
    PharaohMonumentWorkSpotBinding large_bindings[18],
    const PharaohMonumentAnimScript *scripts, int32_t script_count,
    PharaohCrtRandom *random);

void pharaoh_monument_spot_animate(PharaohMonumentSpotAnimation *spot,
    const PharaohMonumentAnimScript *scripts, int32_t script_count,
    const PharaohWalkerQueryRecord *walkers, PharaohCrtRandom *random);
void pharaoh_sphinx_animate_work_spots(PharaohMonumentWorkSpotBinding *bindings,
    const PharaohMonumentAnimScript *scripts, int32_t script_count,
    const PharaohWalkerQueryRecord *walkers, PharaohCrtRandom *random);
void pharaoh_sun_temple_animate_work_spots(PharaohMonumentWorkSpotBinding *bindings,
    const PharaohMonumentAnimScript *scripts, int32_t script_count,
    const PharaohWalkerQueryRecord *walkers, PharaohCrtRandom *random);
void pharaoh_library_animate_work_spots(PharaohMonumentWorkSpotBinding *bindings,
    const PharaohMonumentAnimScript *scripts, int32_t script_count,
    const PharaohWalkerQueryRecord *walkers, PharaohCrtRandom *random);
void pharaoh_abu_simbel_animate_work_spots(PharaohMonumentWorkSpotBinding *bindings,
    const PharaohMonumentAnimScript *scripts, int32_t script_count,
    const PharaohWalkerQueryRecord *walkers, PharaohCrtRandom *random);
void pharaoh_caesareum_animate_work_spots(PharaohMonumentWorkSpotBinding *bindings,
    const PharaohMonumentAnimScript *scripts, int32_t script_count,
    const PharaohWalkerQueryRecord *walkers, PharaohCrtRandom *random);
void pharaoh_lighthouse_animate_work_spots(PharaohMonumentWorkSpotBinding *bindings,
    const PharaohMonumentAnimScript *scripts, int32_t script_count,
    const PharaohWalkerQueryRecord *walkers, PharaohCrtRandom *random);
void pharaoh_royal_tomb_animate_work_spots(
    PharaohMonumentWorkSpotGroup groups[26],
    const PharaohMonumentAnimScript *scripts, int32_t script_count,
    const PharaohWalkerQueryRecord *walkers, PharaohCrtRandom *random);
void pharaoh_obelisk_animate_work_spots(
    const PharaohMonumentAnimationRegistry *registry,
    const PharaohMonumentAnimScript *scripts, int32_t script_count,
    const PharaohWalkerQueryRecord *walkers, PharaohCrtRandom *random);
void pharaoh_monument_animate_all(
    const PharaohMonumentAnimationRegistry *registry,
    const PharaohMonumentAnimScript *scripts, int32_t script_count,
    const PharaohWalkerQueryRecord *walkers, PharaohCrtRandom *random);

#endif
