#ifndef RESTORE_SPRITE_CYCLE_H
#define RESTORE_SPRITE_CYCLE_H
#include <stdint.h>
#include <stddef.h>
typedef struct RestoreSpriteCycle {
    uint8_t prefix[0x34];
    uint32_t flags,source;
    uint16_t action,reserved3e;
    uint32_t descriptor,variant,frame;
    uint32_t playback_rate,action_rate;
    uint8_t unknown54[0x24];
    uint16_t remaining;
} RestoreSpriteCycle;
typedef char restore_sprite_cycle_frame_offset[offsetof(RestoreSpriteCycle,frame)==0x48?1:-1];
typedef char restore_sprite_cycle_remaining_offset[offsetof(RestoreSpriteCycle,remaining)==0x78?1:-1];
void restore_sprite_cycle_set_frame(RestoreSpriteCycle *,int32_t);
void restore_sprite_cycle_select_variant(RestoreSpriteCycle *,uint32_t);
void restore_sprite_cycle_select_frame(RestoreSpriteCycle *,uint32_t,int32_t);
uint32_t restore_sprite_cycle_add_forward(RestoreSpriteCycle *,uint32_t);
uint32_t restore_sprite_cycle_add_reverse(RestoreSpriteCycle *,uint32_t);
uint32_t restore_sprite_cycle_wrap_forward(RestoreSpriteCycle *);
uint32_t restore_sprite_cycle_wrap_reverse(RestoreSpriteCycle *);
uint32_t restore_sprite_cycle_tick(RestoreSpriteCycle *);
uint32_t restore_sprite_cycle_finished(RestoreSpriteCycle *);
uint32_t restore_sprite_cycle_stop_when_finished(RestoreSpriteCycle *);
uint32_t restore_sprite_cycle_hold_last_frame(RestoreSpriteCycle *);

/* Legacy C entries keep their existing stack arguments and cdecl cleanup,
   but capture the member receiver from ECX before any compiler-generated C.
   New C callers use the explicit functions above. No original code executes. */
#define RESTORE_CYCLE_ENTRY0(name,body) \
 __declspec(naked) void name(void) { \
 __asm { push ecx } __asm { call body } __asm { add esp,4 } __asm { ret } }
#define RESTORE_CYCLE_ENTRY1(name,body) \
 __declspec(naked) void name(void) { \
 __asm { push dword ptr[esp+4] } __asm { push ecx } \
 __asm { call body } __asm { add esp,8 } __asm { ret } }
#define RESTORE_CYCLE_ENTRY2(name,body) \
 __declspec(naked) void name(void) { \
 __asm { push dword ptr[esp+8] } __asm { push dword ptr[esp+8] } __asm { push ecx } \
 __asm { call body } __asm { add esp,12 } __asm { ret } }
#endif
