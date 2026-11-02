#ifndef RESTORE_MENU_FULL_INPUT_NUMERIC_H
#define RESTORE_MENU_FULL_INPUT_NUMERIC_H
#include <stdint.h>
/* Explicit typed hardware primitives. No incoming ST0 contract is exposed. */
uint32_t restore_menu_input_length32(int32_t dx,int32_t dy);
uint32_t restore_menu_input_ratio32(int32_t delta,uint32_t length_bits);
void restore_menu_input_coordinate80(int32_t origin,uint32_t ratio_bits,int32_t distance,uint8_t raw80[10]);
uint32_t restore_menu_input_compare32(int32_t integer,uint32_t float_bits);
uint64_t restore_menu_input_trunc80(const uint8_t raw80[10]);
#endif
