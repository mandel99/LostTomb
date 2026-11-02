#ifndef RESTORE_GOD_FROG_EFFECT_H
#define RESTORE_GOD_FROG_EFFECT_H
#include <stdint.h>
/* Logical cdecl/RET0 entries, without incoming ECX. The sole native caller
   discards 4E7300 EAX, including its quotient/void OutputDebugString residue.
   These unique APIs execute the complete new bodies, never legacy entries. */
void restore_god_effect_004e7300(int32_t building);
void restore_god_effect_004e7410(int32_t building,int32_t x,int32_t y);
uint32_t restore_god_effect_004e75a0(int32_t building,int32_t *x,int32_t *y);
void FUN_004e7300(int32_t building);
void FUN_004e7410(int32_t building,int32_t x,int32_t y);
uint32_t FUN_004e75a0(int32_t building,int32_t *x,int32_t *y);
/* Reuses genuine nearest-search and FILD32/FSQRT/raw80/FTOL providers.
   Masked exceptions, inherited PC/RC; FPSW/trap equivalence is not claimed.
   Typed entries do not expose incidental GPRs or discarded caller argslots. */
#endif
