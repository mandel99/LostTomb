#include "restore_types.h"
#define DAT_00d0562c (*(undefined *)0x00d0562cu)
#define DAT_00d05630 (*(undefined *)0x00d05630u)
#define DAT_00d05634 (*(undefined *)0x00d05634u)
#define DAT_00d05638 (*(undefined *)0x00d05638u)
#define DAT_00d0563c (*(undefined *)0x00d0563cu)
extern void FUN_00448a60();
extern int * FUN_0044bb90();

void FUN_004b8c90(void)

{
  int iVar1;
  
  for (iVar1 = FUN_0044bb90(0,1); iVar1 != 0; iVar1 = FUN_0044bb90(iVar1,1)) {
    FUN_00448a60();
  }
  DAT_00d0563c = 0;
  DAT_00d05638 = 0;
  DAT_00d05634 = 0;
  DAT_00d05630 = 0;
  DAT_00d0562c = 0;
  return;
}
