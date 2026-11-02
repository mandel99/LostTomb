#ifndef RESTORE_MAP_REFRESH_ARITHMETIC_H
#define RESTORE_MAP_REFRESH_ARITHMETIC_H
#include <stdint.h>
#include <stddef.h>
typedef struct MRFloat { uint64_t significand;int32_t exponent;uint32_t negative,kind; } MRFloat;
/* kind0: finite, including signed zero; kind1: infinity; kind2: indefinite NaN.
   Nonzero finite value = (-1)^negative * significand * 2^(exponent-63). */
MRFloat mr_fp_i32(int32_t value);
MRFloat mr_fp_binary64(uint64_t bits);
MRFloat mr_fp_add(MRFloat a,MRFloat b,uint16_t cw);
MRFloat mr_fp_sub(MRFloat a,MRFloat b,uint16_t cw);
MRFloat mr_fp_mul(MRFloat a,MRFloat b,uint16_t cw);
MRFloat mr_fp_div(MRFloat a,MRFloat b,uint16_t cw);
uint64_t mr_fp_store64(MRFloat value,uint16_t cw);
void mr_fp_raw80(MRFloat value,uint8_t output[10]);
uint16_t mr_fp_control_word(void);
#ifdef MAP_REFRESH_ARITHMETIC_TRACE
uint64_t map_refresh_arithmetic_read(uint32_t address,uint32_t width);
void map_refresh_arithmetic_write(uint32_t address,uint32_t value);
void map_refresh_arithmetic_fp(uint32_t instruction,MRFloat value);
void map_refresh_arithmetic_spill(uint32_t instruction,uint64_t bits);
void map_refresh_arithmetic_compare(uint32_t instruction,uint32_t c0);
#define MRA_R(a,n) map_refresh_arithmetic_read((uint32_t)(a),(n))
#define MRA_WRITE(a,v) map_refresh_arithmetic_write((a),(v))
#define MRA_FP(a,v) map_refresh_arithmetic_fp((a),(v))
#define MRA_SPILL(a,v) map_refresh_arithmetic_spill((a),(v))
#define MRA_COMPARE(a,v) map_refresh_arithmetic_compare((a),(v))
#else
static __inline uint64_t mra_read(uint32_t a,uint32_t n){if(n==1)return *(volatile uint8_t *)(uintptr_t)a;if(n==2)return *(volatile uint16_t *)(uintptr_t)a;if(n==4)return *(volatile uint32_t *)(uintptr_t)a;return *(volatile uint64_t *)(uintptr_t)a;}
#define MRA_R(a,n) mra_read((uint32_t)(a),(n))
#define MRA_WRITE(a,v) (*(volatile uint32_t *)(uintptr_t)(a)=(uint32_t)(v))
#define MRA_FP(a,v) ((void)0)
#define MRA_SPILL(a,v) ((void)0)
#define MRA_COMPARE(a,v) ((void)0)
#endif
int32_t FUN_00481540(int32_t x,int32_t y,int32_t rings,uint32_t mask);
int32_t FUN_00481690(int32_t x,int32_t y,int32_t rings);
#endif
