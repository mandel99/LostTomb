#ifndef PHARAOH_RUNTIME_MODEL_TEXT_H
#define PHARAOH_RUNTIME_MODEL_TEXT_H

#include <stdint.h>

/* Native loader helper: checks up to 999 bytes for a digit or minus sign. */
uint32_t pharaoh_model_text_skip_to_number(const char *text);

#endif
