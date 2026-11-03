#ifndef RESTORE_HOUSING_CYCLE_H
#define RESTORE_HOUSING_CYCLE_H
#include <stdint.h>
/* Native consumption uses three inherited stack words for negative phases,
   and overwrites its caller's argument word with the linen demand. */
typedef struct restore_house_goods_context {
 uint32_t ebx;
 uint32_t slots[3];
 uint32_t argument_word;
} restore_house_goods_context;
void restore_housing_update_all(void);
void restore_house_repair_position(uint32_t building);
void restore_house_consume_goods(uint32_t building,const restore_house_goods_context *context);
#endif
