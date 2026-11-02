#ifndef RESTORE_SETTINGS_CALLBACKS_H
#define RESTORE_SETTINGS_CALLBACKS_H
#include <stdint.h>
#include "menu_input.h"
#include "sound_streams.h"
#define SSET_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SSET_I(a) ((int32_t)SSET_U(a))
#define SSET_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#ifdef SETTINGS_CALLBACKS_TRACE
void restore_settings_write(uint32_t address,uint32_t width,uint32_t value);
#define SSET_PUT32(a,v) restore_settings_write((uint32_t)(a),4,(uint32_t)(v))
#define SSET_PUT8(a,v) restore_settings_write((uint32_t)(a),1,(uint8_t)(v))
#else
#define SSET_PUT32(a,v) (SSET_U(a)=(uint32_t)(v))
#define SSET_PUT8(a,v) (SSET_B(a)=(uint8_t)(v))
#endif
void FUN_00402c10(void),FUN_00402c40(void),FUN_00402cc0(void);
void FUN_00402e50(void),FUN_00402f70(void),FUN_00403060(void);
void FUN_00403130(void),FUN_004031a0(void),FUN_00403210(void),FUN_00403270(void);
void FUN_004032f0(void),FUN_00403320(void),FUN_00403390(void),FUN_004ea760(void);
void FUN_00416ba0(uint32_t selection);
void restore_settings_sound_55f32f(void *self,int32_t volume);
void restore_settings_sound_55f3eb(void *self,int32_t volume);
void FUN_00540c90(void),FUN_00540ed0(void),FUN_00541160(void),FUN_004bc720(void);
int FUN_004cde40(int first,int second);
#endif
