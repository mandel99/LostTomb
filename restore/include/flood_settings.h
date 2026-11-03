#ifndef RESTORE_FLOOD_SETTINGS_H
#define RESTORE_FLOOD_SETTINGS_H
#include <stdint.h>
/* Settings occupy36 bytes at world+32C40. Initialization writes the first
   eight DWORDs only; the ninth is retained until annual preparation. */
void restore_flood_settings_initialize(uintptr_t self);
void restore_flood_settings_reset(uintptr_t self);
void restore_flood_settings_restore_timing(uintptr_t self);
void restore_flood_settings_reset_quality(uintptr_t self);
void restore_flood_settings_prepare(uintptr_t self);
void restore_flood_settings_roll_quality(uintptr_t self);
void restore_flood_quality_increase(uintptr_t world);
void restore_flood_quality_decrease(uintptr_t world);
int32_t restore_flood_quality(uintptr_t self);
void restore_god_settings_004bd5e0(uint32_t self,int32_t amount);
#define RESTORE_FLOOD_ENTRY0(name,body) \
 __declspec(naked) void name(void) { \
 __asm { push ecx } __asm { call body } __asm { add esp,4 } __asm { ret } }
#define RESTORE_FLOOD_ENTRY1(name,body) \
 __declspec(naked) void name(void) { \
 __asm { push dword ptr[esp+4] } __asm { push ecx } \
 __asm { call body } __asm { add esp,8 } __asm { ret } }
#endif
