#ifndef PHARAOH_RUNTIME_CITY_PLAYER_COUNTDOWN_H
#define PHARAOH_RUNTIME_CITY_PLAYER_COUNTDOWN_H

#include <stdint.h>

/* Native entry 0x004E7E40. The observed field is a signed 16-bit countdown
   in city player slot 1; its wider gameplay meaning is still unresolved. */
void pharaoh_city_player_one_countdown_advance(int16_t *countdown);

#endif
