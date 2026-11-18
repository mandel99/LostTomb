#ifdef NDEBUG
#undef NDEBUG
#endif

#include "pharaoh/runtime/music_population.h"

#include <assert.h>

int main(void)
{
    PharaohMusicManager manager = {0};
    PharaohMusicCategory *category = &manager.categories[3];

    category->track_count = 4;
    category->tracks[0].minimum_population = 0;
    category->tracks[0].maximum_population = 100;
    category->tracks[1].minimum_population = 50;
    category->tracks[1].maximum_population = 200;
    category->tracks[2].minimum_population = 150;
    category->tracks[2].maximum_population = 300;
    category->tracks[3].minimum_population = 301;
    category->tracks[3].maximum_population = 400;

    manager.current_track = 0;
    pharaoh_music_manager_select_population_range(&manager, 3, 175, 1);
    assert(manager.first_track == 1 && manager.last_track == 2);
    assert(manager.current_track == 1 && manager.previous_track == 1);
    assert(manager.phase == 1);

    manager.phase = 9;
    pharaoh_music_manager_select_population_range(&manager, 3, 175, 0);
    assert(manager.first_track == 1 && manager.last_track == 2);
    assert(manager.current_track == 1 && manager.previous_track == 1);
    assert(manager.phase == 9);

    manager.current_track = 2;
    manager.waiting = 1;
    pharaoh_music_manager_select_population_range(&manager, 3, 500, 1);
    assert(manager.first_track == 4 && manager.last_track == 3);
    assert(manager.current_track == 4 && manager.previous_track == 4);
    assert(manager.phase == 9);

    pharaoh_music_manager_set_range(&manager, -2, 7);
    assert(manager.first_track == -2 && manager.last_track == 7);
    return 0;
}
