#ifndef RESTORE_VIEWPORT_CONTROLLER_H
#define RESTORE_VIEWPORT_CONTROLLER_H
#include <stdint.h>
void FUN_00430d30(uint32_t camera_x,uint32_t camera_y,uint32_t camera_mode);
void FUN_004dddc0(void);
/* Native direct callees, all cdecl; no ECX receiver. Algorithms are outside
   this controller proof and will be controlled endpoints in the oracle. */
void FUN_00430ef0(void);
void FUN_004316f0(void);
void FUN_00436b60(uint32_t);
void FUN_0051f780(void);
void FUN_00439f10(void);
void FUN_0043b630(void);
void FUN_00439740(uint32_t);
void FUN_0043e300(void);
#ifdef RESTORE_VIEWPORT_CONTROLLER_TRACE
uint32_t restore_viewport_controller_read(uint32_t address,uint32_t width);
void restore_viewport_controller_write(uint32_t address,uint32_t value);
static __inline uint32_t viewport_read32(uint32_t a){return restore_viewport_controller_read(a,4);}
static __inline uint8_t viewport_read8(uint32_t a){return (uint8_t)restore_viewport_controller_read(a,1);}
static __inline void viewport_write32(uint32_t a,uint32_t v){restore_viewport_controller_write(a,v);}
#else
static __inline uint32_t viewport_read32(uint32_t a){return *(volatile uint32_t *)(uintptr_t)a;}
static __inline uint8_t viewport_read8(uint32_t a){return *(volatile uint8_t *)(uintptr_t)a;}
static __inline void viewport_write32(uint32_t a,uint32_t v){*(volatile uint32_t *)(uintptr_t)a=v;}
#endif
#endif
