/* Exact function text extracted from current ROOT; guarded below. */
#include "restore_types.h"
#include <stdint.h>
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
extern void *FUN_0041e100(void),*FUN_0041e130(void);
extern uint32_t FUN_0041e140(void);
extern unsigned char FUN_0041e170(void);
extern void FUN_00425730(int,int,void *,int),FUN_0041f220(void);
void restore_mission_menu_buttons(void)
{
    B(0xe38dc9)=1;
    FUN_00425730(I(0xe38da8),I(0xe38da4),FUN_0041e100(),FUN_0041e170());
    if(FUN_0041e140())FUN_00425730(I(0xe38da8),I(0xe38da4),FUN_0041e130(),2);
    FUN_0041f220();
}
