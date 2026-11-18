#include "pharaoh/runtime/city_player_countdown.h"

void pharaoh_city_player_one_countdown_advance(int16_t *countdown)
{
    if (*countdown > 0) {
        --*countdown;
    }
}
