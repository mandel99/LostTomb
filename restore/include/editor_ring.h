#ifndef RESTORE_EDITOR_RING_H
#define RESTORE_EDITOR_RING_H
#include <stdint.h>
typedef uint8_t (__cdecl *EditorRingVisitor)(uint32_t linear,int32_t x,int32_t y,uint32_t context);
void FUN_004ed0d0(uint32_t center,int32_t radius,EditorRingVisitor visitor,uint32_t context);
void FUN_00405f60(void);
void FUN_004ed330(void);
uint8_t __cdecl FUN_004ed2d0(uint32_t linear,int32_t x,int32_t y,uint32_t context);
#define ERR32(a) (*(volatile uint32_t *)(uintptr_t)(a))
#define ERW32(a,v) (*(volatile uint32_t *)(uintptr_t)(a)=(uint32_t)(v))
#endif
