#include "restore_types.h"
#define DAT_005c4358 (*(undefined4 *)0x005c4358u)
#define DAT_005c45a4 (*(undefined4 *)0x005c45a4u)
#define DAT_005c47f0 (*(undefined4 *)0x005c47f0u)
#define DAT_006059fc (*(undefined *)0x006059fcu)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00e38e68 (*(undefined *)0x00e38e68u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56fa4 (*(undefined *)0x00f56fa4u)
#define DAT_00f56fac (*(undefined *)0x00f56facu)
extern void FUN_004cd1a0();
extern void FUN_004d6200();
extern void FUN_004d6380();
extern void FUN_00536820();

void FUN_00520d00(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined *puVar3;
  int iVar4;
  
  if (DAT_00e38e68 == 1) {
    uVar1 = 0x6e;
    uVar2 = 0x1e;
  }
  else if (DAT_00e38e68 == 2) {
    uVar1 = 0xbe;
    uVar2 = 0x5a;
  }
  else {
    uVar1 = 0x12e;
    uVar2 = 0xae;
  }
  FUN_00536820(uVar1,uVar2,0x1a,0x1c);
  iVar4 = 0;
  puVar3 = &DAT_005c4358;
  do {
    if (((puVar3 == &DAT_005c4358) || (puVar3 == &DAT_005c45a4)) ||
       ((uVar1 = DAT_00f56fac, iVar4 + 1 != DAT_007056c8 &&
        (uVar1 = DAT_00f56fa4, iVar4 != DAT_006059fc)))) {
      uVar1 = DAT_00f56f8c;
    }
    FUN_004d6200(0xc5,iVar4,puVar3,uVar1,0,0x73,0x1e,DAT_00e38da8,DAT_00e38da4);
    puVar3 = puVar3 + 0x1c;
    iVar4 = iVar4 + 1;
  } while ((int)puVar3 < 0x5c47f0);
  FUN_004cd1a0(0,&DAT_005c47f0,DAT_007056c8 == 0x2b,0x6e,0x1e);
  FUN_004d6380(0x12,3,&DAT_005c47f0,DAT_00f56f8c,0,0x72,0x22,DAT_00e38da8,DAT_00e38da4);
  return;
}
