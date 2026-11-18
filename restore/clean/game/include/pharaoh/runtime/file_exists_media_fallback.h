#ifndef PHARAOH_RUNTIME_FILE_EXISTS_MEDIA_FALLBACK_H
#define PHARAOH_RUNTIME_FILE_EXISTS_MEDIA_FALLBACK_H

#include <stdint.h>

#include "pharaoh/runtime/file_directory_reset.h"
#include "pharaoh/runtime/file_media_root.h"
#include "pharaoh/runtime/file_size.h"

typedef struct PharaohFileExistsMediaFallbackContext {
    const PharaohFileSizeContext *files;
    PharaohFileMediaRootContext *media_root;
    const PharaohFileDirectoryResetContext *directory_reset;
} PharaohFileExistsMediaFallbackContext;

/* Native entry 0x004CE3F0. Probes in the current directory, changes to the
   selected media root after the first miss, retries, then restores startup
   directory state before returning the retry result. */
int32_t pharaoh_file_exists_with_media_fallback(
    const PharaohFileExistsMediaFallbackContext *context,
    const char *filename);

#endif
