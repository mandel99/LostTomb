#ifndef PHARAOH_RENDERING_MONUMENT_SCENE_DATA_H
#define PHARAOH_RENDERING_MONUMENT_SCENE_DATA_H

#include "pharaoh/rendering/monument_spot_animation.h"

extern const PharaohMonumentAnimScript pharaoh_monument_animation_scripts[
    PHARAOH_MONUMENT_ANIMATION_SCRIPT_COUNT];

typedef struct PharaohMonumentSceneData {
    PharaohMonumentWorkSpotBinding small_obelisk[10], large_obelisk[18];
    PharaohMonumentWorkSpotBinding sphinx[22], sun_temple[13];
    PharaohMonumentWorkSpotBinding library[27], abu_simbel[56];
    PharaohMonumentWorkSpotBinding caesareum[27], lighthouse[15];
    PharaohMonumentWorkSpotBinding royal_tomb[75];
    PharaohMonumentWorkSpotGroup royal_groups[25];
} PharaohMonumentSceneData;

/* Once per fresh scene, before reading saved state. Pointers must name arrays
 * of 10/18/22/13/27/56/27/15/75 live spots, in the field order above.
 * The bindings borrow these arrays; loaded spot state is never reset here. */
void pharaoh_monument_scene_data_initialize(PharaohMonumentSceneData *data,
    PharaohMonumentSpotAnimation *const spots[9]);

#endif
