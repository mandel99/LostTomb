#ifndef PHARAOH_RUNTIME_MUSIC_POPULATION_H
#define PHARAOH_RUNTIME_MUSIC_POPULATION_H

#include "pharaoh/runtime/music_manager.h"

_Static_assert(offsetof(PharaohMusicTrack, minimum_population) == 0x68,
    "native music-track minimum-population offset");
_Static_assert(offsetof(PharaohMusicTrack, maximum_population) == 0x6c,
    "native music-track maximum-population offset");
_Static_assert(offsetof(PharaohMusicManager, first_track) == 0x59a54,
    "native music-manager first-track offset");
_Static_assert(offsetof(PharaohMusicManager, current_track) == 0x59a60,
    "native music-manager current-track offset");
_Static_assert(offsetof(PharaohMusicManager, waiting) == 0x59a6c,
    "native music-manager waiting offset");

/* Original address 0x00539A50. */
void pharaoh_music_manager_set_range(PharaohMusicManager *manager,
    int32_t first_track, int32_t last_track);

/* Original address 0x00539730. population and category are the typed values
   read from the active city player and current music category by the EXE. */
void pharaoh_music_manager_select_population_range(
    PharaohMusicManager *manager, int32_t category, int32_t population,
    int32_t force);

#endif
