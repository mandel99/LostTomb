#ifndef RESTORE_TRAINING_PANEL_H
#define RESTORE_TRAINING_PANEL_H
#include "restore_types.h"
#include <stdint.h>
#define TP_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define TP_S(a) ((int32_t)*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#ifdef TRAINING_PANEL_TRACE
void restore_training_panel_write(uint32_t address,uint32_t value);
#define TP_PUT(a,v) restore_training_panel_write((uint32_t)(a),(uint32_t)(v))
#else
#define TP_PUT(a,v) (TP_U(a)=(uint32_t)(v))
#endif
/* Complete native CFG never reads the caller-pushed context DWORD. */
void FUN_0055ad30(void);
#endif
