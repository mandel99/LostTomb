#ifndef RESTORE_COMMAND_MIGRATION_H
#define RESTORE_COMMAND_MIGRATION_H
#include <stdint.h>
/* All entries are cdecl/RET0; no incoming ECX receiver. 469D50 defines AL,
   and preserves a building-record pointer in EAX[31:8] on its early true path. */
void FUN_0041cca0(void);
void FUN_00469c60(int32_t repetitions);
uint32_t FUN_00469d50(void);
void FUN_00469e40(int32_t building,uint8_t alternative);
uint32_t FUN_0047fe60(int32_t x,int32_t y,uint32_t size,int32_t distance);
uint32_t FUN_0047feb0(int32_t x,int32_t y,uint32_t size,int32_t distance,uint32_t skip);
int restore_migration_command_action(uint32_t token);
#endif
