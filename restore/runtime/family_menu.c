#include "family_menu.h"
#include <string.h>
extern void FUN_004cc500(int);
extern void FUN_004cc3c0(void);
extern void FUN_004cc3e0(void);
extern void FUN_004264d0(uint32_t,uint32_t,uint32_t);
/* Original local callbacks omitted from the exported procedure list. */
void FUN_00516c10(void)
{
    memset((void *)0xe38e74u,0,32);
    *(uint8_t *)0xe38ddau=0x36;*(uint32_t *)0xe20998u=0;
    FUN_004cc500(1);FUN_004cc3c0();FUN_004cc3e0();
    *(uint32_t *)0xe94dc0u=0x2f;*(uint8_t *)0xe38dcau=1;
}
void FUN_00516c60(void)
{FUN_004264d0(0x5a,0x516dd0,2);}
