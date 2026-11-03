#ifndef RESTORE_HOUSE_LAYOUT_H
#define RESTORE_HOUSE_LAYOUT_H
#include "house_levels.h"
uint32_t restore_house_try_merge(uint32_t building);
void restore_house_apply_layout(uint32_t native_target,uint32_t building,uint32_t population);
/* Typed native-compatible split provider for overlapping houses. */
void restore_house_split_for_expansion(uint32_t building,uint32_t native_target);
#endif
