#ifndef RESTORE_FORMATION_CONTROL_H
#define RESTORE_FORMATION_CONTROL_H
#include <stdint.h>
uint32_t restore_formation_create(uint32_t building);
void restore_formation_recruit(int32_t count,uint32_t formation,int32_t x,int32_t y);
void restore_formation_refresh(void);
#endif
