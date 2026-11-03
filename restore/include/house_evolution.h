#ifndef RESTORE_HOUSE_EVOLUTION_H
#define RESTORE_HOUSE_EVOLUTION_H
#include <stdint.h>
/* Original4539A0,453AF0,453B40. All use ordinary caller-cleanup C ABI. */
uint32_t restore_house_check_global_mode(void);
int32_t restore_house_compare_desirability(uint32_t building);
uint32_t restore_house_check_requirements(uint32_t building,uint32_t next_level);
#endif
