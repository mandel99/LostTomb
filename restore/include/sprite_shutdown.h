#ifndef RESTORE_SPRITE_SHUTDOWN_H
#define RESTORE_SPRITE_SHUTDOWN_H
#include "sprite_libraries.h"
#include <stdint.h>
uint32_t FUN_0053db60(RestoreSpriteLibraries *self);
RestoreSpriteLibraries *FUN_0053da90(RestoreSpriteLibraries *self),*FUN_0053da60(void);
void FUN_0053db00(RestoreSpriteLibraries *self),FUN_0053da80(void),FUN_00540500(RestoreSpriteLibrary *self);
RestoreSpriteLibrary *FUN_005404e0(RestoreSpriteLibrary *self);
int FUN_0053da70(void);
int FUN_0053da50(void);
void restore_sprite_libraries_destroy_records(RestoreSpriteLibraries *);
void restore_sprite_libraries_destroy_frame(uint32_t);
#define RESTORE_SPRITE_FRAME_ENTRY(name) \
 __declspec(naked) void name(void) { \
 __asm { push ebp } __asm { call restore_sprite_libraries_destroy_frame } \
 __asm { add esp,4 } __asm { ret } }
void FUN_004e63b0(void);
void FUN_0053f790(int *slot,char clear_sentinel);
void restore_sprite_shutdown_free(void *allocation),restore_sprite_shutdown_delete(void *object,uint32_t flags),restore_sprite_shutdown_slot(int *slot);
int restore_sprite_shutdown_register(void (*callback)(void));
#endif
