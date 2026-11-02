#ifndef RESTORE_CAMERA_PAN_H
#define RESTORE_CAMERA_PAN_H
#include <stdint.h>
#include "camera_bounds.h"
#define CP_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
int32_t FUN_004cf660(void);
uint32_t FUN_004ddd60(void);
void FUN_004ddde0(int32_t direction);
void FUN_0041d6f0(void);
#endif
