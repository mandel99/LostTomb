#include "restore_types.h"
#define DAT_005c79c8 (*(undefined *)0x005c79c8u)
#define DAT_005c79cc (*(undefined *)0x005c79ccu)
#define DAT_005c79e4 (*(undefined *)0x005c79e4u)
#define DAT_00705698 (*(undefined *)0x00705698u)
#define DAT_007056b8 (*(undefined *)0x007056b8u)
#define DAT_00d3b198 (*(undefined *)0x00d3b198u)
#define DAT_00e38dc9 (*(char *)0x00e38dc9u)
#define DAT_00e38e34 (*(undefined *)0x00e38e34u)
extern void FUN_00425900();
extern int FUN_00425990();

undefined4 FUN_00425840(void)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = DAT_007056b8;
  DAT_00705698 = 0;
  iVar2 = FUN_00425990(0,0,&DAT_005c79c8,1);
  if (iVar2 != 0) {
    DAT_007056b8 = 0x14;
    if (DAT_00e38e34 < (int)DAT_005c79cc / 3 + (int)DAT_005c79c8) {
      DAT_005c79e4 = 1;
    }
    else {
      DAT_005c79e4 = (((DAT_005c79cc * 2) / 3 + (int)DAT_005c79c8 <= DAT_00e38e34) - 1 & 0xfffffffe)
                     + 2;
    }
    DAT_00e38dc9 = 1;
    DAT_00d3b198 = DAT_005c79e4;
    FUN_00425900();
    DAT_00705698 = DAT_00d3b198 + 1;
    return 1;
  }
  DAT_007056b8 = uVar1;
  return 0;
}
