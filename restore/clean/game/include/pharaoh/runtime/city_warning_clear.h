#ifndef PHARAOH_RUNTIME_CITY_WARNING_CLEAR_H
#define PHARAOH_RUNTIME_CITY_WARNING_CLEAR_H

#include <stdint.h>

#include "pharaoh/city/warning_types.h"

enum { PHARAOH_CITY_WARNING_WINDOW_WIDTH = 180 };

typedef struct PharaohCityWarningClearContext {
    int32_t screen_width;
    int32_t *warning_state;
    PharaohCityWarningPositions *warning_positions;
    PharaohCityWarningTable *warnings;
} PharaohCityWarningClearContext;

void pharaoh_city_warning_clear_all(
    const PharaohCityWarningClearContext *context);

#endif
