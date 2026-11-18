#include "pharaoh/runtime/autosave_dispatch.h"

enum {
    AUTOSAVE_PATH_CAPACITY = 100,
    AUTOSAVE_STATE_REPLAY = 0,
    AUTOSAVE_STATE_HISTORY = 2,
    AUTOSAVE_STATE_SAVING = 1,
    AUTOSAVE_DISABLED_EVENT_VALUE = 1
};

static const char autosave_path_separator[] = "\\";
static const char autosave_replay_filename[] = "autosave_replay.sav";
static const char autosave_history_filename[] = "autosave_history.sav";
static const char autosave_filename[] = "autosave.sav";

static char *copy_native_string(char *destination, const char *source)
{
    do {
        *destination = *source;
        if (*source == '\0') {
            return destination;
        }
        ++destination;
        ++source;
    } while (1);
}

static char *append_native_string(char *destination, const char *source)
{
    while (*destination != '\0') {
        ++destination;
    }
    return copy_native_string(destination, source);
}

void pharaoh_autosave_dispatch(
    const PharaohAutosaveDispatchState *state,
    const PharaohAutosaveDispatchOperations *operations)
{
    int32_t save_state;
    int32_t replay_mode;
    const char *filename;
    char path[AUTOSAVE_PATH_CAPACITY];
    char *path_end;

    if (*state->event_54_suppress_flood ==
            AUTOSAVE_DISABLED_EVENT_VALUE) {
        return;
    }

    save_state = *state->current_player_save_state;
    if (save_state == AUTOSAVE_STATE_SAVING) {
        return;
    }
    if (save_state == AUTOSAVE_STATE_HISTORY &&
        *state->save_state_blocker != 0) {
        return;
    }

    replay_mode = *state->replay_save_mode;
    if (*state->replay_save_mode == 1) {
        filename = save_state == AUTOSAVE_STATE_REPLAY
            ? autosave_replay_filename
            : autosave_history_filename;
    } else {
        filename = autosave_filename;
    }

    *state->current_player_save_state = AUTOSAVE_STATE_SAVING;
    path_end = copy_native_string(path,
        operations->obtain_save_path_prefix(operations->user_data));
    path_end = append_native_string(path_end, autosave_path_separator);
    (void)append_native_string(path_end, filename);
    operations->write_mission_save(operations->user_data, path);

    if (replay_mode == 1) {
        operations->store_replay_save_path(operations->user_data, path);
        operations->commit_replay_save_resource(operations->user_data,
            operations->get_replay_save_resource(operations->user_data));
    }
}
