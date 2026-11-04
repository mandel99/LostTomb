/* Retained pre-fix compilation model from src/unit_02809.c. Analysis only;
   never included in CMake. The independent entry did not initialize the old
   private heap, and its generated HANDLE binding was a single byte. */
#include "restore_types.h"
#define DAT_00604c54 (*(undefined *)0x00604c54u)
#define DAT_01266a7c (*(undefined *)0x01266a7cu)
extern int *FUN_005657bb();
void *FUN_00561ba6(uint param_1)
{
    int *piVar1;
    if((param_1<=DAT_00604c54)&&(piVar1=FUN_005657bb(param_1),piVar1!=0))return piVar1;
    if(param_1==0)param_1=1;
    return HeapAlloc(DAT_01266a7c,0,(param_1+15)&0xfffffff0);
}
