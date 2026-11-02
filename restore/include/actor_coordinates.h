#ifndef RESTORE_ACTOR_COORDINATES_H
#define RESTORE_ACTOR_COORDINATES_H
#include <stdint.h>
#define AC_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define AC_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define AC_S(a) ((int32_t)*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
static __inline int32_t ac_add(int32_t a,int32_t b){return(int32_t)((uint32_t)a+(uint32_t)b);}
static __inline int32_t ac_sub(int32_t a,int32_t b){return(int32_t)((uint32_t)a-(uint32_t)b);}
static __inline int32_t ac_mul(int32_t a,int32_t b){return(int32_t)((uint32_t)a*(uint32_t)b);}
static __inline uint32_t ac_out(const int32_t *p){return*(const volatile uint32_t *)p;}
static __inline void ac_store(int32_t *p,uint32_t value){*(volatile uint32_t *)p=value;}
void FUN_004f08c0(int32_t building,int32_t step,int32_t *x,int32_t *y);
void FUN_004f0ab0(int32_t row,int32_t column,int32_t *map_y,int32_t *map_x);
void FUN_004f0ba0(int32_t x,int32_t y,int32_t *screen_x,int32_t *screen_y);
void FUN_004f0d30(int32_t actor,int32_t *x,int32_t *y);
#endif
