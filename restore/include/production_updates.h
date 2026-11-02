#ifndef RESTORE_PRODUCTION_UPDATES_H
#define RESTORE_PRODUCTION_UPDATES_H
#include <stdint.h>
/* Native 488630 receives its ten-byte schedule in ECX. */
void FUN_00488630(volatile uint8_t *schedule);
void FUN_00455900(void);
uint8_t FUN_00488350(int32_t building);
uint32_t FUN_00459240(int32_t building);
/* Native4611D0 is cdecl/RET0; its results are two bytes and one DWORD. */
void FUN_004611d0(int32_t first,int32_t second,int32_t count,uint8_t *first_slot,uint8_t *second_slot,uint32_t *action);
#endif
