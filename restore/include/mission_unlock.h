#ifndef RESTORE_MISSION_UNLOCK_H
#define RESTORE_MISSION_UNLOCK_H
#include <stdint.h>
uint32_t FUN_00403ef0(void);
void FUN_00404350(void);
void FUN_004051a0(void);
void FUN_004179f0(void);
void FUN_00417a70(void);
void FUN_00417aa0(void);
/* Explicit body contract; raw_stack is the incoming 32-byte scratch prestate. */
void restore_mission_unlock_body(uint8_t raw_stack[32]);
/* Public original no-argument ABI. */
void FUN_00417b40(void);
int32_t FUN_0044bd00(void);
void FUN_0053b090(int32_t sound);
#endif
