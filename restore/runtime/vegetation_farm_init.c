#include "vegetation_farm_init.h"
#include <stdint.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
/* Native CRT57604C ->486080 ->486090. Initial growth rates and the four
   pending/scan flags are separate from serialized terrain cell storage. */
void FUN_00486090(void)
{
    U(0x978d88u)=25;U(0x978d8cu)=35;
    U(0x978d90u)=0;U(0x978d94u)=0;U(0x978d98u)=0;
}
void FUN_00486080(void){FUN_00486090();}

/* Native CRT576050 ->489940 ->489950. The immutable binary64 constant
   at56F5A8 is exactly10.0. Truncation to integer, two LEA multiplications
   by5, and SHL3 produce2000 in every supported x87 rounding mode. */
void FUN_00489950(void){U(0xbca34cu)=2000;}
void FUN_00489940(void){FUN_00489950();}
