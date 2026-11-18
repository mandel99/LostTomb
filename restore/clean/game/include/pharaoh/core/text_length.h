#ifndef PHARAOH_CORE_TEXT_LENGTH_H
#define PHARAOH_CORE_TEXT_LENGTH_H

#include <stdint.h>

/* Native 0x004CC000 stops after examining 10,000 bytes. */
int32_t pharaoh_text_length(const char *text);

#endif
