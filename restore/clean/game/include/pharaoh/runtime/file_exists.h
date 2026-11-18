#ifndef PHARAOH_RUNTIME_FILE_EXISTS_H
#define PHARAOH_RUNTIME_FILE_EXISTS_H

#include <stdint.h>

#include "pharaoh/runtime/file_directory_reset.h"
#include "pharaoh/runtime/file_size.h"

/* Native entry 0x004CE450. Restores the saved directory before probing the
   filename and returns one only when the native open helper returns >= 0. */
int32_t pharaoh_file_exists(const PharaohFileSizeContext *files,
                            const PharaohFileDirectoryResetContext *directory,
                            const char *filename);

#endif
