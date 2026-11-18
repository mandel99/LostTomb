#ifndef PHARAOH_RUNTIME_APPLICATION_WINDOW_STARTUP_H
#define PHARAOH_RUNTIME_APPLICATION_WINDOW_STARTUP_H

#include "pharaoh/runtime/application_display_startup.h"
#include "pharaoh/runtime/application_startup.h"
#include "pharaoh/runtime/application_window.h"
#include "pharaoh/runtime/video_player.h"
#include "pharaoh/runtime/random_generator.h"

enum {
    PHARAOH_APPLICATION_WINDOW_RESET_FLAG_COUNT = 4,
    PHARAOH_APPLICATION_WINDOW_RESET_WORD_COUNT = 4,
    PHARAOH_APPLICATION_VIDEO_PLAYER_NATIVE_BYTES = 0x1c
};

typedef PharaohRandomGeneratorValues PharaohApplicationRandomGeneratorState;

typedef struct PharaohApplicationWindowResetState {
    /* These slots retain the native widths/order; their wider game meanings
       have not been established from the current type export. */
    uint8_t pre_window_flags[PHARAOH_APPLICATION_WINDOW_RESET_FLAG_COUNT];
    uint8_t pre_window_status;
    int32_t pre_window_words[PHARAOH_APPLICATION_WINDOW_RESET_WORD_COUNT];
} PharaohApplicationWindowResetState;

typedef struct PharaohApplicationWindowStartupState {
    PharaohApplicationDirectoryState directory;
    PharaohApplicationWindowState window;
    PharaohInitialWindowGeometry geometry;
    PharaohApplicationRandomGeneratorState random_generator;
    PharaohApplicationWindowResetState reset_state;
    PharaohVideoPlayer *video_player;
} PharaohApplicationWindowStartupState;

typedef struct PharaohApplicationWindowStartupRequest {
    void *instance;
    int32_t settings_display_mode;
    PharaohTextSelection *window_title_selection;
    PharaohApplicationWindowProcedure window_procedure;
} PharaohApplicationWindowStartupRequest;

typedef struct PharaohApplicationWindowStartupOperations {
    void *user_data;
    const PharaohApplicationDirectoryOperations *directory_operations;
    const PharaohGraphicsStartupOperations *graphics_operations;
    const PharaohApplicationWindowOperations *window_operations;
    int32_t (*co_initialize)(void *user_data, void *reserved);
    PharaohVideoPlayer *(*allocate_video_player)(void *user_data,
                                                 uint32_t native_byte_count);
} PharaohApplicationWindowStartupOperations;

/* Original entry 0x004CF960. The WndProc is a typed C callback input; this
   owner contains no executable-address or Restore-frame dispatch. */
int32_t pharaoh_application_window_initialize(
    PharaohApplicationWindowStartupState *state,
    PharaohGraphicsStartupState *graphics,
    const PharaohApplicationWindowStartupRequest *request,
    const PharaohApplicationWindowStartupOperations *operations);

#endif
