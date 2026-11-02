#include "restore_types.h"
#define DAT_005dbf70 (*(undefined4 *)0x005dbf70u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00af55fc (*(undefined4 *)0x00af55fcu)
#define DAT_00af5988 (*(undefined4 *)0x00af5988u)
#define DAT_00af5990 (*(undefined4 *)0x00af5990u)
#define DAT_00af5d1c (*(undefined4 *)0x00af5d1cu)
#define DAT_00b4e534 (*(undefined4 *)0x00b4e534u)
#define DAT_00b4e6fa (*(undefined4 *)0x00b4e6fau)
#define DAT_00b4e6fe (*(undefined4 *)0x00b4e6feu)
#define DAT_00b4e8c4 (*(undefined4 *)0x00b4e8c4u)
#define DAT_00bc97c0 (*(undefined *)0x00bc97c0u)
#define DAT_00bc97c4 (*(undefined *)0x00bc97c4u)
#define DAT_00bc97c8 (*(undefined *)0x00bc97c8u)
#define DAT_00bc97cc (*(undefined *)0x00bc97ccu)
#define DAT_00bc97d0 (*(undefined *)0x00bc97d0u)
#define DAT_00bc97d4 (*(undefined *)0x00bc97d4u)
#define DAT_00bc97d8 (*(undefined *)0x00bc97d8u)
#define DAT_00bc97dc (*(undefined *)0x00bc97dcu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
extern uint FUN_0047e230();

uint FUN_0047e340(int param_1,uint param_2)

{
  short sVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  bool bVar7;
  int aiStack_20 [8];
  
  iVar3 = param_1;
  iVar4 = param_2 * 0x184;
  bVar7 = (&DAT_00bcd602)[iVar4] != 0x62;
  DAT_00bc97dc = 0;
  DAT_00bc97d4 = 0;
  DAT_00bc97cc = 0;
  DAT_00bc97d0 = *(uint *)(&DAT_00af5d1c + param_1 * 4) & 0x440;
  DAT_00bc97c4 = 0;
  DAT_00bc97c0 = *(uint *)(&DAT_00af55fc + param_1 * 4) & 0x440;
  DAT_00bc97c8 = *(uint *)(&DAT_00af5990 + param_1 * 4) & 0x440;
  DAT_00bc97d8 = *(uint *)(&DAT_00af5988 + param_1 * 4) & 0x440;
  if (((*(uint *)(&DAT_00af55fc + param_1 * 4) & 8) != 0) &&
     ((((sVar1 = (&DAT_007999e8)[*(short *)(&DAT_00b4e534 + param_1 * 2) * 0x84], sVar1 == 0xab ||
        (sVar1 == 0xca)) || ((sVar1 == 0x8a && (bVar7)))) || (sVar1 == 0x88)))) {
    DAT_00bc97c0 = 0;
  }
  if (((*(uint *)(&DAT_00af5990 + param_1 * 4) & 8) != 0) &&
     (((sVar1 = (&DAT_007999e8)[*(short *)(&DAT_00b4e6fe + param_1 * 2) * 0x84], sVar1 == 0xab ||
       (sVar1 == 0xca)) || (((sVar1 == 0x8a && (bVar7)) || (sVar1 == 0x88)))))) {
    DAT_00bc97c8 = 0;
  }
  if (((*(uint *)(&DAT_00af5d1c + param_1 * 4) & 8) != 0) &&
     ((((sVar1 = (&DAT_007999e8)[*(short *)(&DAT_00b4e8c4 + param_1 * 2) * 0x84], sVar1 == 0xab ||
        (sVar1 == 0xca)) || ((sVar1 == 0x8a && (bVar7)))) || (sVar1 == 0x88)))) {
    DAT_00bc97d0 = 0;
  }
  if (((*(uint *)(&DAT_00af5988 + param_1 * 4) & 8) != 0) &&
     (((sVar1 = (&DAT_007999e8)[*(short *)(&DAT_00b4e6fa + param_1 * 2) * 0x84], sVar1 == 0xab ||
       (sVar1 == 0xca)) || (((sVar1 == 0x8a && (bVar7)) || (sVar1 == 0x88)))))) {
    DAT_00bc97d8 = 0;
  }
  param_2 = (uint)(DAT_00bc97c0 != 0);
  if (DAT_00bc97c8 != 0) {
    param_2 = (DAT_00bc97c0 != 0) + 1;
  }
  if (DAT_00bc97d0 != 0) {
    param_2 = param_2 + 1;
  }
  if (DAT_00bc97d8 != 0) {
    param_2 = param_2 + 1;
  }
  if (2 < param_2) {
    iVar5 = 7;
    iVar6 = 0;
    param_1 = 0;
    uVar2 = *(undefined4 *)(&DAT_005dbf70 + (uint)(byte)(&DAT_00bcd602)[iVar4] * 4);
    do {
      if (*(int *)((int)&DAT_00bc97c0 + iVar6) != 0) {
        iVar4 = FUN_0047e230(uVar2,*(int *)(iVar6 + 0x5dc128) + iVar3);
        *(int *)((int)aiStack_20 + iVar6) = iVar4;
        if (iVar4 <= iVar5) {
          iVar5 = iVar4;
        }
        if (param_1 <= iVar4) {
          param_1 = iVar4;
        }
      }
      iVar6 = iVar6 + 8;
    } while (iVar6 < 0x20);
    iVar4 = 0;
    do {
      if ((*(int *)((int)&DAT_00bc97c0 + iVar4) != 0) && (iVar5 < *(int *)((int)aiStack_20 + iVar4))
         ) {
        *(undefined4 *)((int)&DAT_00bc97c0 + iVar4) = 0;
        param_2 = param_2 - 1;
      }
      iVar4 = iVar4 + 8;
    } while (iVar4 < 0x20);
  }
  return param_2;
}
