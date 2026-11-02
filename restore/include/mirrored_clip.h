#ifndef RESTORE_MIRRORED_CLIP_H
#define RESTORE_MIRRORED_CLIP_H
#include <stdint.h>
void FUN_0040fcf0(const void *pixels);
void FUN_0040fdd0(const void *pixels);
char *FUN_00410f20(char *pixels);
#define MC_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define MC_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define MC_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#ifdef RESTORE_MIRRORED_CLIP_TRACE
void restore_mirrored_clip_finish(uint32_t source);
void restore_mirrored_clip_pixel(uint32_t address,uint16_t value);
#endif
#endif
