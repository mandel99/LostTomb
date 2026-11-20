#ifndef PHARAOH_UI_MAP_SELECTION_RATING_H
#define PHARAOH_UI_MAP_SELECTION_RATING_H

#include <stddef.h>
#include <stdint.h>

typedef struct PharaohMapSelectionRatingEvent {
    uint8_t reserved_00[2];
    uint8_t kind;
    uint8_t reserved_03[0x79];
} PharaohMapSelectionRatingEvent;

_Static_assert(offsetof(PharaohMapSelectionRatingEvent, kind) == 2 &&
               sizeof(PharaohMapSelectionRatingEvent) == 0x7c,
               "scenario event rating view");

typedef struct PharaohMapSelectionRatingInput {
    /* Native WORD/DWORD fields at 0x784C24, 0x784C28, 0x784C2C, 0x784C30. */
    int32_t rating_terms[4];
    int32_t population_rating_enabled;
    int32_t population;
    int32_t invasion_count;
    const PharaohMapSelectionRatingEvent *events;
    int32_t event_count;
    const int16_t *god_settings;
} PharaohMapSelectionRatingInput;

/* Native 0x0041E470. */
int32_t pharaoh_map_selection_rating(
    const PharaohMapSelectionRatingInput *input);

#endif
