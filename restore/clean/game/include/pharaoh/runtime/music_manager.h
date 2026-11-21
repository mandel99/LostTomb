#ifndef PHARAOH_RUNTIME_MUSIC_MANAGER_H
#define PHARAOH_RUNTIME_MUSIC_MANAGER_H

#include <stddef.h>
#include <stdint.h>

#include "pharaoh/core/crt_random.h"

typedef struct PharaohMusicTrack {
    char path[100];
    int32_t delay;
    int32_t minimum_population;
    int32_t maximum_population;
} PharaohMusicTrack;

typedef struct PharaohMusicCategory {
    PharaohMusicTrack tracks[60];
    int32_t ambient_after[50];
    int32_t track_count;
    int32_t ambient_count;
} PharaohMusicCategory;

typedef struct PharaohMusicManager {
    PharaohMusicCategory categories[53];
    int32_t enabled;
    int32_t first_track;
    int32_t last_track;
    int32_t previous_track;
    int32_t current_track;
    int32_t phase;
    uint32_t tick;
    int32_t waiting;
    int32_t reserved;
} PharaohMusicManager;

_Static_assert(sizeof(PharaohMusicTrack) == 0x70,
               "music track native stride");
_Static_assert(offsetof(PharaohMusicCategory, track_count) == 0x1b08,
               "music category track-count offset");
_Static_assert(sizeof(PharaohMusicCategory) == 0x1b10,
               "music category native stride");
_Static_assert(offsetof(PharaohMusicManager, enabled) == 0x59a50,
               "music enabled native offset");
_Static_assert(sizeof(PharaohMusicManager) == 0x59a74,
               "music manager native size");

/* Native entry 0x00539A40 writes the enabled field and returns void. */
void pharaoh_music_manager_set_enabled(PharaohMusicManager *manager,
                                      int32_t enabled);

/* Complete C translations of Pharaoh.exe 0x00539550 and 0x005395E0. */
PharaohMusicManager *pharaoh_music_manager_construct(
    PharaohMusicManager *manager);
int32_t pharaoh_music_manager_add_track(PharaohMusicManager *manager,
    const char *path, int32_t category, int32_t delay,
    int32_t minimum_population, int32_t maximum_population, int32_t ambient);

typedef struct PharaohMusicUpdateState {
    PharaohMusicManager *manager;
    PharaohCrtRandom *random;
    int32_t sound_ready;
    uint8_t window_active;
    int8_t category_index;
    int32_t population;
    int32_t volume;
    uint32_t delay_ms;
    void *user_data;
    void (*set_volume)(void *user_data, int32_t volume);
    void (*play_stream)(void *user_data, const char *path,
        int32_t parameter1, int32_t parameter2);
    uint32_t (*milliseconds)(void *user_data);
} PharaohMusicUpdateState;

/* Complete C translation of Pharaoh.exe 0x00539820. */
void pharaoh_music_manager_update(PharaohMusicUpdateState *state);

#endif
