#ifndef PHARAOH_RUNTIME_APPLICATION_WINDOW_ACTIVATION_H
#define PHARAOH_RUNTIME_APPLICATION_WINDOW_ACTIVATION_H

#include <stdint.h>
#include "pharaoh/runtime/digital_sound_stream.h"

typedef struct PharaohApplicationWindowActivateAudioContext {
    int32_t *foreground_restore_pending;
    PharaohDigitalSoundDeviceState *sound_manager;
    const PharaohDigitalSoundStreamCloseOperations *sound_operations;
} PharaohApplicationWindowActivateAudioContext;

/* Implements the native WM_ACTIVATE branch when its high-word minimized
   flag is set. The focus-restoration branch remains in the host proc. */
int32_t pharaoh_application_window_handle_deactivate_audio(
    uint32_t message, uintptr_t wparam,
    const PharaohApplicationWindowActivateAudioContext *context);

#endif
