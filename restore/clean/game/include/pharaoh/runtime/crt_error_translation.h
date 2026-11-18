#ifndef PHARAOH_RUNTIME_CRT_ERROR_TRANSLATION_H
#define PHARAOH_RUNTIME_CRT_ERROR_TRANSLATION_H

#include <stdint.h>

typedef struct PharaohCrtErrorState {
    uint32_t dos_error;
    int32_t errno_value;
} PharaohCrtErrorState;

/* Native entry 0x005672C8: stores the Win32 error and maps it to CRT errno. */
void pharaoh_crt_error_from_win32(PharaohCrtErrorState *state,
                                  uint32_t win32_error);

#endif
