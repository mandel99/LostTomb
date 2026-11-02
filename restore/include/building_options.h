#ifndef RESTORE_BUILDING_OPTIONS_H
#define RESTORE_BUILDING_OPTIONS_H
#include <stdint.h>
/* 4F5050 preserves the original high EAX carry; only AL is Boolean. */
uint32_t FUN_004f5050(uint32_t resource,uint8_t force);
uint32_t FUN_005150a0(uint32_t kind);
void FUN_00515130(uint32_t output[15]);
#endif
