#ifndef PHARAOH_SIMULATION_FARM_INITIALIZATION_H
#define PHARAOH_SIMULATION_FARM_INITIALIZATION_H

#include <stdint.h>

/* Native00489950 and its CRT thunk00489940. The binary64 constant10.0,
 * FTOL and exact integer multiplications yield2000 in every x87 PC/RC mode.
 * The observed CRT consumer discards EAX; no incidental result is exposed.
 * This is the unsaved program owner BCA34C, not a per-frame derived value. */
void pharaoh_farm_max_progress_initialize(int32_t *maximum);
void pharaoh_farm_max_progress_construct(int32_t *maximum);

#endif
