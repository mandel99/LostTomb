#ifndef RESTORE_HOUSE_LEVELS_H
#define RESTORE_HOUSE_LEVELS_H
#include <stdint.h>
/* Twenty nullary native callbacks. The owning4528B0 dispatcher discards
   volatile return registers; these callbacks have a void contract. */
void restore_house_update_level(uint32_t level);
void restore_house_update_by_entry(uint32_t native_entry);
/* Typed providers for housing size and footprint transitions. */
void restore_house_merge_neighbours(uint32_t building);
uint32_t restore_house_can_expand(uint32_t building,uint32_t area);
void restore_house_clear_expansion(uint32_t building,uint32_t area);
uint32_t restore_house_prepare_expansion(uint32_t building,uint32_t area);
void restore_house_expand(uint32_t building,uint32_t layout,uint32_t parcels);
void restore_house_reduce(uint32_t building,uint32_t layout);
#endif
