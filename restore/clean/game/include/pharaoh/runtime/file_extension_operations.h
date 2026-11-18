#ifndef PHARAOH_RUNTIME_FILE_EXTENSION_OPERATIONS_H
#define PHARAOH_RUNTIME_FILE_EXTENSION_OPERATIONS_H

#include "pharaoh/runtime/file_path_state.h"

/* Native entry 0x004CE310 copies three caller bytes into the persistent
   extension state, then applies the native replacement operation. */
void pharaoh_file_change_extension(PharaohFilePathState *state,
                                   char *path,
                                   const char extension[3]);

/* Native entry 0x004CE370 replaces the three bytes after the first dot when
   one exists, and writes a NUL one byte after them. */
void pharaoh_file_replace_extension(PharaohFilePathState *state,
                                    char *path);

/* Native entry 0x004CE3A0 truncates a path at its first dot. */
void pharaoh_file_remove_extension(char *path);

/* Native entry 0x004CE3C0 appends dot, three stored extension bytes, and NUL. */
void pharaoh_file_append_extension(PharaohFilePathState *state,
                                   char *path);

#endif
