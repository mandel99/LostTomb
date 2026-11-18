#include "pharaoh/runtime/music_population.h"

void pharaoh_music_manager_set_range(PharaohMusicManager *manager,
    int32_t first_track, int32_t last_track)
{
    manager->first_track = first_track;
    manager->last_track = last_track;
}

void pharaoh_music_manager_select_population_range(
    PharaohMusicManager *manager, int32_t category_index, int32_t population,
    int32_t force)
{
    PharaohMusicCategory *category = &manager->categories[category_index];
    PharaohMusicTrack *current_track =
        &category->tracks[manager->current_track];

    if (current_track->maximum_population < population ||
        population < current_track->minimum_population || force != 0) {
        int32_t first = 0;
        int32_t last;
        int32_t track_count = category->track_count;

        while (first < track_count) {
            const PharaohMusicTrack *track = &category->tracks[first];
            if (track->minimum_population <= population &&
                population <= track->maximum_population) {
                break;
            }
            ++first;
        }

        last = first;
        while (last < track_count &&
            category->tracks[last].minimum_population <= population) {
            ++last;
        }
        pharaoh_music_manager_set_range(manager, first, last - 1);
    }

    if (manager->current_track < manager->first_track ||
        manager->current_track > manager->last_track) {
        manager->current_track = manager->first_track;
    }

    if (manager->first_track != manager->previous_track) {
        manager->previous_track = manager->first_track;
        if (manager->waiting == 0) {
            manager->phase = 1;
        }
    }
}
