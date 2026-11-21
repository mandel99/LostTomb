#include "pharaoh/runtime/music_manager.h"
#include "pharaoh/runtime/music_population.h"

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

static int32_t music_look_ahead(const PharaohMusicCategory *category,
    int32_t index)
{
    int32_t value;
    const unsigned char *address = (const unsigned char *)category +
        offsetof(PharaohMusicCategory, ambient_after) +
        (uint32_t)index * sizeof(int32_t);
    memcpy(&value, address, sizeof(value));
    return value;
}

void pharaoh_music_manager_update(PharaohMusicUpdateState *state)
{
    PharaohMusicManager *manager = state->manager;
    PharaohMusicCategory *category;

    if (state->sound_ready == 0 || state->window_active == 0)
        return;

    if (manager->phase == 0) {
        pharaoh_music_manager_select_population_range(manager,
            state->category_index, state->population, 0);
    }
    state->set_volume(state->user_data, state->volume);

    switch (manager->phase) {
    case 0:
        category = &manager->categories[state->category_index];
        if (category->track_count > 0) {
            state->play_stream(state->user_data,
                category->tracks[manager->current_track].path, 0, 1);
            category = &manager->categories[state->category_index];
            state->delay_ms =
                (uint32_t)category->tracks[manager->current_track].delay * 1000u;
            manager->waiting = 0;
            manager->current_track =
                (int32_t)((uint32_t)manager->current_track + 1u);
            manager->phase = music_look_ahead(category,
                manager->current_track) != 1 ? 2 : 1;
        }
        break;
    case 1:
        category = &manager->categories[state->category_index];
        if (category->ambient_count > 0) {
            const uint32_t random = pharaoh_crt_random_next(state->random);
            const int32_t product = (int32_t)(random *
                (uint32_t)category->ambient_count);
            const int32_t ambient = product / 32768;
            state->play_stream(state->user_data,
                category->tracks[50 + ambient].path, 0, 1);
        }
        manager->phase = 2;
        break;
    case 2:
        manager->phase = 3;
        manager->tick = state->milliseconds(state->user_data);
        break;
    case 3:
    {
        const uint32_t now = state->milliseconds(state->user_data);
        if (manager->tick + state->delay_ms < now) {
            if (manager->current_track > manager->last_track)
                manager->current_track = manager->first_track;
            manager->phase = 0;
            manager->waiting = 1;
        }
        break;
    }
    default:
        break;
    }
}
