#include "restore_types.h"
#define DAT_01266a84 (*(undefined *)0x01266a84u)
#define DAT_01267aa4 (*(undefined *)0x01267aa4u)
extern undefined4 FUN_005697ef();
extern void FUN_00569bf5();

byte * FUN_0056793c(void)

{
  byte bVar1;
  int iVar2;
  byte *pbVar3;
  byte *pbVar4;
  
  if (DAT_01266a84 == 0) {
    FUN_00569bf5();
  }
  bVar1 = *(undefined4 *)(DAT_01267aa4);
  pbVar4 = DAT_01267aa4;
  if (bVar1 == 0x22) {
    while( true ) {
      pbVar3 = pbVar4;
      bVar1 = pbVar3[1];
      pbVar4 = pbVar3 + 1;
      if ((bVar1 == 0x22) || (bVar1 == 0)) break;
      iVar2 = FUN_005697ef(bVar1);
      if (iVar2 != 0) {
        pbVar4 = pbVar3 + 2;
      }
    }
    if (*pbVar4 == 0x22) goto LAB_00567979;
  }
  else {
    while (0x20 < bVar1) {
      bVar1 = pbVar4[1];
      pbVar4 = pbVar4 + 1;
    }
  }
  for (; (*pbVar4 != 0 && (*pbVar4 < 0x21)); pbVar4 = pbVar4 + 1) {
LAB_00567979: ;
  }
  return pbVar4;
}
