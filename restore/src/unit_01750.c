#include "restore_types.h"
#define ram0x00e36c9a (*(undefined4 *)0x00e36c9au)
#define DAT_00e34238 (*(undefined *)0x00e34238u)
#define DAT_00e36c90 (*(undefined *)0x00e36c90u)
#define DAT_00e36c94 (*(undefined *)0x00e36c94u)
#define DAT_00e36c98 (*(undefined *)0x00e36c98u)
#define DAT_00e36c99 (*(undefined4 *)0x00e36c99u)
#define DAT_00e36f10 (*(undefined *)0x00e36f10u)
#define DAT_00e36f14 (*(undefined *)0x00e36f14u)
#define DAT_00e36f18 (*(undefined *)0x00e36f18u)
#define DAT_00e36f1a (*(undefined *)0x00e36f1au)
#define DAT_00e36f1e (*(undefined *)0x00e36f1eu)
#define DAT_00e36f22 (*(undefined *)0x00e36f22u)
#define DAT_00e92e48 (*(undefined *)0x00e92e48u)
#define _DAT_00e36c98 (*(undefined4 *)0x00e36c98u)
#define _DAT_00e36ccc (*(undefined4 *)0x00e36cccu)
extern int FUN_004cc5c0();
extern void FUN_004ce860();
extern void FUN_004ce8e0();

void FUN_004ce6e0(char *param_1,int param_2,int param_3)

{
  undefined4 uVar1;
  undefined2 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined4 *puVar6;
  
  FUN_004ce8e0();
  if (0 < param_3) {
    FUN_004cc5c0(param_3 + -1,0,0);
  }
  if (DAT_00e92e48 != 0) {
    DAT_00e36c94 = DAT_00e36f1e;
    _DAT_00e36c98 = DAT_00e36f22;
    DAT_00e36c90 = DAT_00e36f1a;
    ram0x00e36c9a = 0x2020;
    iVar3 = 0xc;
    iVar4 = 0;
    do {
      if (param_1[iVar4] == '\0') break;
      *(char *)((int)&DAT_00e36c90 + iVar3) = param_1[iVar4];
      iVar3 = iVar3 + 1;
      iVar4 = iVar4 + 1;
    } while (iVar4 < 100);
    if (param_2 != 0) {
      iVar4 = 0;
      do {
        if (*(char *)(iVar4 + param_2) == '\0') break;
        *(char *)((int)&DAT_00e36c90 + iVar3) = *(char *)(iVar4 + param_2);
        iVar3 = iVar3 + 1;
        iVar4 = iVar4 + 1;
      } while (iVar4 < 100);
      *(undefined2 *)((int)&DAT_00e36c90 + iVar3) = 0x2020;
      iVar3 = iVar3 + 2;
    }
    if (param_3 != 0) {
      iVar4 = 0;
      do {
        if ((&DAT_00e34238)[iVar4] == '\0') break;
        *(undefined1 *)((int)&DAT_00e36c90 + iVar3) = (&DAT_00e34238)[iVar4];
        iVar3 = iVar3 + 1;
        iVar4 = iVar4 + 1;
      } while (iVar4 < 100);
      *(undefined2 *)((int)&DAT_00e36c90 + iVar3) = 0x2020;
      iVar3 = iVar3 + 2;
    }
    *(undefined2 *)((int)&DAT_00e36c90 + iVar3) = 0xa0d;
    FUN_004ce860(iVar3 + 2);
    return;
  }
  iVar3 = 0;
  iVar4 = 0;
  iVar5 = (int)&DAT_00e36c90 - (int)param_1;
  do {
    if (*param_1 == '\0') break;
    iVar3 = iVar3 + 1;
    param_1[iVar5] = *param_1;
    iVar4 = iVar4 + 1;
    param_1 = param_1 + 1;
  } while (iVar4 < 100);
  uVar1 = DAT_00e36f14;
  *(undefined4 *)((int)&DAT_00e36c90 + iVar3) = DAT_00e36f10;
  uVar2 = DAT_00e36f18;
  *(undefined4 *)((int)&DAT_00e36c94 + iVar3) = uVar1;
  *(undefined2 *)(&DAT_00e36c98 + iVar3) = uVar2;
  *(undefined2 *)((int)&DAT_00e36c99 + iVar3 + 1) = 0xa0d;
  FUN_004ce860(iVar3 + 0xc);
  puVar6 = &DAT_00e36c90;
  for (iVar3 = 0xf; iVar3 != 0; iVar3 = iVar3 + -1) {
    *puVar6 = 0x2d2d2d2d;
    puVar6 = puVar6 + 1;
  }
  _DAT_00e36ccc = 0xa0d;
  FUN_004ce860(0x3e);
  return;
}
