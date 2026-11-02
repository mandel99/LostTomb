#include "restore_types.h"
#define DAT_006049c0 (*(undefined4 *)0x006049c0u)
#define DAT_006049e0 (*(undefined4 *)0x006049e0u)
#define DAT_00604e70 (*(undefined4 *)0x00604e70u)
#define DAT_01266960 (*(uint32_t *)0x01266960u)
extern DWORD FUN_005662e3();
extern int FUN_005664d5();
extern void FUN_0056917b();
extern byte FUN_005691bf();

uint FUN_00564ab1(uint param_1,int *param_2)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  undefined *puVar6;
  
  piVar4 = param_2;
  uVar1 = param_2[3];
  uVar2 = param_2[4];
  if (((uVar1 & 0x82) == 0) || ((uVar1 & 0x40) != 0)) {
LAB_00564bba: ;
    param_2[3] = uVar1 | 0x20;
  }
  else {
    if ((uVar1 & 1) != 0) {
      param_2[1] = 0;
      if ((uVar1 & 0x10) == 0) goto LAB_00564bba;
      *param_2 = param_2[2];
      param_2[3] = uVar1 & 0xfffffffe;
    }
    uVar1 = param_2[3];
    param_2[1] = 0;
    param_2 = (int *)0x0;
    piVar4[3] = uVar1 & 0xffffffef | 2;
    if (((uVar1 & 0x10c) == 0) &&
       (((piVar4 != (int *)&DAT_006049c0 && (piVar4 != (int *)&DAT_006049e0)) ||
        (iVar5 = FUN_005691bf(uVar2), iVar5 == 0)))) {
      FUN_0056917b(piVar4);
    }
    if ((*(ushort *)(piVar4 + 3) & 0x108) == 0) {
      iVar5 = 1;
      param_2 = (int *)FUN_005664d5(uVar2,&param_1,1);
    }
    else {
      iVar3 = piVar4[2];
      iVar5 = *piVar4 - iVar3;
      *piVar4 = iVar3 + 1;
      piVar4[1] = piVar4[6] + -1;
      if (iVar5 < 1) {
        if (uVar2 == 0xffffffff) {
          puVar6 = &DAT_00604e70;
        }
        else {
          puVar6 = (undefined *)((&DAT_01266960)[(int)uVar2 >> 5] + (uVar2 & 0x1f) * 8);
        }
        if ((puVar6[4] & 0x20) != 0) {
          FUN_005662e3(uVar2,0,2);
        }
      }
      else {
        param_2 = (int *)FUN_005664d5(uVar2,iVar3,iVar5);
      }
      *(undefined1 *)piVar4[2] = (undefined1)param_1;
    }
    if (param_2 == (int *)iVar5) {
      return param_1 & 0xff;
    }
    piVar4[3] = piVar4[3] | 0x20;
  }
  return 0xffffffff;
}
