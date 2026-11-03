#ifndef RESTORE_SUN_TEMPLE_H
#define RESTORE_SUN_TEMPLE_H
#include "building_methods.h"
/* Native5417B0/541920 are ordinary cdecl3/cdecl1, both RET0.
   The static layout contains100 records of52 bytes; no ECX receiver. */
void restore_sun_temple_create(int32_t x,int32_t y,uint32_t root);
void restore_sun_temple_map(uint32_t building);
void restore_sun_temple_frame(uint32_t target,struct restore_menu_input_frame *frame);
#endif
