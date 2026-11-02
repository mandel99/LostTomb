#ifndef RESTORE_TRAINING_QUERIES_H
#define RESTORE_TRAINING_QUERIES_H
#include "restore_types.h"
#include <stdint.h>
#define TQ_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define TQ_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define TQ_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define TQ_S(a) ((int32_t)*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#ifndef TQ_PUBLIC
#define TQ_PUBLIC(name) name
#endif
uint32_t FUN_00443950(int resource),FUN_004439c0(int resource),FUN_00443a20(int resource),FUN_00443b20(int resource),FUN_00443b90(int resource),FUN_00443d90(int resource);
bool FUN_00447790(int record,int resource);
uint32_t FUN_004c5190(void);
int FUN_004c5240(int value);
#endif
