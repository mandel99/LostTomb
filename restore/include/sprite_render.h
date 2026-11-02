#ifndef RESTORE_SPRITE_RENDER_H
#define RESTORE_SPRITE_RENDER_H
#include "sprite_libraries.h"
/* Original callers all bind ECX to the global manager. The recovered C
   symbol remains a three-argument facade over a testable explicit receiver. */
int restore_sprite_draw(RestoreSpriteLibraries *self,int image,int x,int y);
int FUN_0053e590(int image,int x,int y);
void FUN_004cdad0(void);
void FUN_004cdb80(void);
void FUN_0040fb60(const void *pixels);
void FUN_0040ffd0(const void *pixels);
void FUN_00410090(const void *pixels);
void restore_sprite_copy(const void *pixels,int color_key,int clipped);
void FUN_0040f9f0(uint32_t *destination,int32_t source_skip,int32_t destination_skip);
void FUN_0040f9a0(const uint32_t *pixels,int32_t destination_skip,int32_t source_skip);
void FUN_0040fed0(const void *pixels);
void FUN_0040ff50(const void *pixels);
void FUN_00410b10(const void *pixels);
void FUN_00410230(const void *pixels);
void restore_sprite_rle(const void *pixels,int solid);
#endif
