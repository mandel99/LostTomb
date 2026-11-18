#ifndef PHARAOH_RUNTIME_FILE_PATH_HELPERS_H
#define PHARAOH_RUNTIME_FILE_PATH_HELPERS_H

#include <stdint.h>

/* Native entry 0x004CE330. If a dot is found, copies the following three
   bytes and a NUL to extension; if no dot is found, leaves it untouched. */
void pharaoh_file_path_capture_extension(const char *path,
                                        char extension[4]);

/* Native entry 0x004CBEC0. Uppercases ASCII a-z in a writable NUL-terminated
   byte string and stores every visited non-NUL byte back to the string. */
void pharaoh_ascii_uppercase_in_place(uint8_t *text);

#endif
