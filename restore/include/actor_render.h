#ifndef RESTORE_ACTOR_RENDER_H
#define RESTORE_ACTOR_RENDER_H
#include <stdint.h>
#include "actor_predicate.h"
#include "sprite_render.h"
#define AR_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define AR_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define AR_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define AR_SB(a) ((int32_t)(int8_t)AR_B(a))
#define AR_SW(a) ((int32_t)(int16_t)AR_W(a))
#define AR_S(v) ((int32_t)(uint32_t)(v))
void FUN_00438e50(int32_t actor_id,uint32_t x,uint32_t y,uint32_t selection);
void FUN_00439740(uint32_t selection);
void FUN_004f0d30(int32_t actor_id,uint32_t *x,uint32_t *y);
void FUN_004cce00(uint32_t value,uint8_t separator,const char *suffix,uint32_t x,uint32_t y,uint32_t font,uint32_t color);
static __inline uint32_t actor_render_meta(uint32_t image,uint32_t limit,uint32_t field)
{
    uint32_t index=AR_S(image)<AR_S(limit)?image:0;
    return (uint32_t)AR_SW(field+index*64u);
}
#endif
