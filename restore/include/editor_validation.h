#ifndef RESTORE_EDITOR_VALIDATION_H
#define RESTORE_EDITOR_VALIDATION_H
#include <stdint.h>
#include "restore_types.h"
#include "file_list.h"
#include "text_selection.h"
#define EV_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define EV_I(a) ((int32_t)EV_U(a))
#define EV_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define EV_S(a) ((int32_t)*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define EV_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#ifndef EV_PUBLIC
#define EV_PUBLIC(name) name
#endif
#ifdef EDITOR_VALIDATION_TRACE
void restore_editor_validation_write(uint32_t address,uint32_t width,uint32_t value);
#define EV_PUT32(a,v) restore_editor_validation_write((uint32_t)(a),4,(uint32_t)(v))
#define EV_PUT8(a,v) restore_editor_validation_write((uint32_t)(a),1,(uint8_t)(v))
#else
#define EV_PUT32(a,v) (EV_U(a)=(uint32_t)(v))
#define EV_PUT8(a,v) (EV_B(a)=(uint8_t)(v))
#endif
void FUN_00409830(void);
uint8_t FUN_004ec360(void),FUN_004095c0(void),FUN_00409650(void),FUN_004096e0(void),FUN_00409760(void),FUN_004097c0(void);
uint8_t FUN_00409620(uint32_t terrain);
bool FUN_00409820(void);
uint32_t FUN_004ec410(uint32_t terrain,char mode);
int FUN_00443710(void);
void FUN_004cdff0(uint32_t unused,char maps),FUN_004ce3a0(char *name);
int FUN_00538cd0(uint32_t first,uint32_t second,uint32_t third);
#endif
