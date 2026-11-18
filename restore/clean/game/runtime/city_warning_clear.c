#include "pharaoh/runtime/city_warning_clear.h"

void pharaoh_city_warning_clear_all(
    const PharaohCityWarningClearContext *context)
{
    const int32_t centered_x =
        (context->screen_width - PHARAOH_CITY_WARNING_WINDOW_WIDTH) / 2;

    for (int32_t state = 0; state < PHARAOH_CITY_WARNING_COUNT; ++state) {
        context->warning_state[state] = 0;
    }

    for (int32_t warning = 0; warning < PHARAOH_CITY_WARNING_COUNT; ++warning) {
        (*context->warning_positions)[warning][0] = centered_x;
        (*context->warnings)[warning].warning_id = (int16_t)centered_x;
        (*context->warnings)[warning].x = (int16_t)
            (*context->warning_positions)[warning][1];
        (*context->warnings)[warning].y = 0;
    }
}
