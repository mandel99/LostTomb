#ifndef RESTORE_EDITOR_BRUSH_VALIDATION_H
#define RESTORE_EDITOR_BRUSH_VALIDATION_H
#include <stdint.h>
void FUN_00405f80(void);
void FUN_00405f60(void);
#define EBV_U(a) (*(volatile uint32_t *)(uintptr_t)(a))
#define EBV_S(a) (*(volatile int16_t *)(uintptr_t)(a))
#ifdef EDITOR_BRUSH_TRACE
void editor_brush_store(uint32_t address,uint32_t value,uint32_t size);
#define EBV_W(a,v) editor_brush_store((uint32_t)(a),(uint32_t)(v),4)
#define EBV_WB(a,v) editor_brush_store((uint32_t)(a),(uint8_t)(v),1)
#define EBV_WS(a,v) editor_brush_store((uint32_t)(a),(uint16_t)(v),2)
#else
#define EBV_W(a,v) (EBV_U(a)=(uint32_t)(v))
#define EBV_WB(a,v) (*(volatile uint8_t *)(uintptr_t)(a)=(uint8_t)(v))
#define EBV_WS(a,v) (*(volatile uint16_t *)(uintptr_t)(a)=(uint16_t)(v))
#endif
uint32_t FUN_004ecec0(int32_t center,int32_t tool,uint32_t *failure_flags);
uint8_t FUN_004ecea0(int32_t tool);
void FUN_004ed360(int32_t x,int32_t y,int32_t mode,uint32_t mask);
uint32_t FUN_00485100(int32_t record);
uint32_t FUN_0046f9a0(int32_t x,int32_t y,int32_t margin);
void FUN_0040f860(void *source,void *destination,int32_t bytes);
void FUN_004f0ba0(int32_t x,int32_t y,int32_t *screen_x,int32_t *screen_y);
int32_t FUN_0053f4f0(int32_t image,int32_t x,int32_t y,uint32_t mask);
#endif
