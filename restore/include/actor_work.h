#ifndef RESTORE_ACTOR_WORK_H
#define RESTORE_ACTOR_WORK_H
#include <stdint.h>
/* Explicit cdecl actor/work entries; member animation methods stay raw. */
void FUN_004b1df0(int32_t actor);
void FUN_004b1ef0(int32_t actor,int32_t steps);
int32_t FUN_004b3ff0(int32_t actor,int32_t mode);
void FUN_004b4180(int32_t actor);
void FUN_004b43a0(int32_t actor);
void FUN_004b43e0(int32_t actor);
void FUN_004200f0(int32_t building);
void FUN_004200b0(void);
void FUN_0041fd80(void *record,int32_t actor);
void FUN_00420130(uint32_t unused,int32_t actor,uint8_t next_state);
int32_t FUN_00420350(uint32_t unused1,uint32_t unused2,int32_t actor);
int32_t FUN_00420c40(int32_t building,int32_t actor);
int32_t FUN_004bc140(int32_t x,int32_t y,int32_t actor);
void FUN_00420a90(int32_t building,uint32_t descriptor);
void FUN_00420b10(int32_t building,int32_t actor,uint8_t state);
void FUN_00420dc0(int32_t building);
void FUN_00420e20(int32_t building,int32_t actor,uint8_t state);
void FUN_004210c0(int32_t building);
void FUN_00421120(int32_t building,int32_t actor,uint8_t state);
int32_t FUN_004e5c00(uint32_t unused1,uint32_t unused2,int32_t actor);
void FUN_004e5d10(int32_t building,int32_t actor,uint8_t state);
void FUN_004ef200(int32_t building);
void FUN_004ef3a0(int32_t building,int32_t actor);
uint32_t FUN_004f01e0(uint32_t unused1,uint32_t unused2,uint32_t actor);
void FUN_00420240(int32_t building,int32_t actor,uint8_t state);
void FUN_00420770(int32_t building);
void FUN_004207e0(int32_t building,int32_t actor,uint8_t state);
int32_t FUN_00422a90(int32_t building,int32_t actor);
void FUN_00542dd0(int32_t building);
void FUN_004ee630(int32_t building);
void FUN_004ee5d0(int32_t building);
#endif
