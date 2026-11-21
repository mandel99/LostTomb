#include "pharaoh/runtime/music_manager.h"

#include <string.h>

void pharaoh_music_manager_set_enabled(PharaohMusicManager *manager,
                                       int32_t enabled)
{
    manager->enabled = enabled;
}

PharaohMusicManager *pharaoh_music_manager_construct(
    PharaohMusicManager *manager)
{
    int32_t *tail = &manager->enabled;
    int32_t index;

    for (index = 0; index < 9; ++index)
        tail[index] = 0;
    for (index = 0; index < 53; ++index) {
        PharaohMusicCategory *category = &manager->categories[index];
        category->track_count = 0;
        memset(category->tracks, 0, sizeof(category->tracks));
        memset(category->ambient_after, 0, sizeof(category->ambient_after));
        /* Native 0x00539550 intentionally leaves ambient_count untouched. */
    }
    return manager;
}

static void copy_track_path(char *destination, const char *source)
{
    do {
        *destination = *source;
        ++destination;
    } while (*source++ != '\0');
}

int32_t pharaoh_music_manager_add_track(PharaohMusicManager *manager,
    const char *path, int32_t category_index, int32_t delay,
    int32_t minimum_population, int32_t maximum_population, int32_t ambient)
{
    PharaohMusicCategory *category;
    PharaohMusicTrack *track;
    int32_t index;

    if (category_index > 53)
        return -1;
    category = (PharaohMusicCategory *)((unsigned char *)manager +
        category_index * (int32_t)sizeof(PharaohMusicCategory));

    if (ambient != 0) {
        index = category->ambient_count;
        if (index == 10)
            return -1;
        track = &category->tracks[50 + index];
        copy_track_path(track->path, path);
        track->delay = delay;
        track->minimum_population = 0;
        track->maximum_population = 0;
        ++category->ambient_count;
    } else {
        index = category->track_count;
        if (index == 50)
            return -1;
        track = &category->tracks[index];
        copy_track_path(track->path, path);
        track->delay = delay;
        track->minimum_population = minimum_population;
        track->maximum_population = maximum_population;
        ++category->track_count;
    }
    return 1;
}
