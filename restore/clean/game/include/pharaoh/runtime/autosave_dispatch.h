#ifndef PHARAOH_RUNTIME_AUTOSAVE_DISPATCH_H
#define PHARAOH_RUNTIME_AUTOSAVE_DISPATCH_H

#include <stdint.h>

typedef struct PharaohAutosaveDispatchState {
    const int32_t *event_54_suppress_flood;
    int32_t *current_player_save_state;
    const int32_t *save_state_blocker;
    const uint8_t *replay_save_mode;
} PharaohAutosaveDispatchState;

typedef struct PharaohAutosaveDispatchOperations {
    void *user_data;
    char *(*obtain_save_path_prefix)(void *user_data);
    void (*write_mission_save)(void *user_data, const char *path);
    void (*store_replay_save_path)(void *user_data, const char *path);
    uint32_t (*get_replay_save_resource)(void *user_data);
    void (*commit_replay_save_resource)(void *user_data, uint32_t resource);
} PharaohAutosaveDispatchOperations;

/* Typed translation of native autosave dispatch 0x004D5C30. */
void pharaoh_autosave_dispatch(
    const PharaohAutosaveDispatchState *state,
    const PharaohAutosaveDispatchOperations *operations);

#endif
