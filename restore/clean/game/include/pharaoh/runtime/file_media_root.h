#ifndef PHARAOH_RUNTIME_FILE_MEDIA_ROOT_H
#define PHARAOH_RUNTIME_FILE_MEDIA_ROOT_H

#include <stdint.h>

#include "pharaoh/runtime/crt_error_translation.h"
#include "pharaoh/runtime/file_path_state.h"

typedef struct PharaohFileMediaRootContext {
    PharaohFilePathState *path_state;
    char media_drive_letter;
    PharaohCrtErrorState *crt_error_state;
} PharaohFileMediaRootContext;

/* Native entry 0x004CE100. Selects the media-drive root only for WAV, SMK,
   and 555 paths. The extension and 40-byte fallback path are persistent
   native scratch state and must be shared by all callers. */
int32_t pharaoh_file_change_to_media_root(
    PharaohFileMediaRootContext *context, const char *path);

#endif
