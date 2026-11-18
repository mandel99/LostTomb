#ifndef PHARAOH_RUNTIME_CURRENT_DIRECTORY_H
#define PHARAOH_RUNTIME_CURRENT_DIRECTORY_H

#include <stdint.h>

#include "pharaoh/runtime/crt_error_translation.h"

/* Native entry 0x0056B2DD. On failure, writes the translated Win32 error to
   the supplied CRT error state and returns -1; success returns zero. */
int32_t pharaoh_set_current_directory(PharaohCrtErrorState *crt_error_state,
                                      const char *directory);

#endif
