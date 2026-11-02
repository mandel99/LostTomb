#include "restore_types.h"
#define DAT_0070e440 (*(undefined *)0x0070e440u)
#define DAT_0070e444 (*(undefined *)0x0070e444u)
#define DAT_0070e448 (*(undefined *)0x0070e448u)
#define DAT_0070e44c (*(undefined *)0x0070e44cu)
#define DAT_0070e450 (*(undefined *)0x0070e450u)
#define DAT_0070e454 (*(undefined *)0x0070e454u)
#define DAT_0070e458 (*(undefined *)0x0070e458u)
#define DAT_0070e45c (*(undefined *)0x0070e45cu)
#define DAT_0070e488 (*(undefined *)0x0070e488u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799ad8 (*(undefined4 *)0x00799ad8u)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00d3b5c0 (*(undefined *)0x00d3b5c0u)
#define DAT_00f5644c (*(undefined *)0x00f5644cu)
extern undefined4 FUN_00442280();
extern int FUN_00475b50();
extern int FUN_00488030();
#include "world_building_draw.h"
extern int FUN_0053e590();
extern undefined4 FUN_0053f250();

void FUN_00435e70(int param_1,int param_2,int param_3)

{
  byte bVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  
  iVar3 = FUN_00475b50(param_1);
  if (*(int *)(&DAT_00799ad8 + iVar3 * 0x108) == 0) {
    return;
  }
  bVar1 = (&DAT_00b285cc)[DAT_0070e488];
  DAT_00d3b5c0 = (&DAT_00b7482c)[DAT_0070e488];
  uVar5 = bVar1 & 0xf;
  iVar6 = param_2;
  iVar4 = param_3;
  if ((bVar1 & 0xf) != 0) {
    if (uVar5 == 1) {
      iVar6 = DAT_0070e45c + 1 + param_2;
      iVar4 = param_3 - DAT_0070e44c;
    }
    else if (uVar5 == 2) {
      iVar6 = DAT_0070e458 + 2 + param_2;
      iVar4 = param_3 - DAT_0070e448;
    }
    else if (uVar5 == 3) {
      iVar6 = DAT_0070e454 + 3 + param_2;
      iVar4 = param_3 - DAT_0070e444;
    }
    else if (uVar5 == 4) {
      iVar6 = DAT_0070e450 + 4 + param_2;
      iVar4 = param_3 - DAT_0070e440;
    }
    else {
      if (uVar5 != 5) goto LAB_00435f80;
      iVar6 = DAT_0070e450 + DAT_0070e45c + 5 + param_2;
      iVar4 = (param_3 - DAT_0070e440) - DAT_0070e44c;
    }
  }
  FUN_0053f250(DAT_00d3b5c0,iVar6,iVar4);
LAB_00435f80: ;
  if (*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) == '\0') {
    if ((&DAT_007999e8)[param_1 * 0x84] == 0x47) {
      FUN_00442280(param_1,DAT_0070e488,param_2,param_3);
    }
    else {
      cVar2 = FUN_00488030(param_1);
      if (cVar2 == '\0') {
        if ((0x40 < (short)(&DAT_007999e8)[iVar3 * 0x84]) &&
           ((short)(&DAT_007999e8)[iVar3 * 0x84] < 0x46)) {
          if ((bVar1 & 0xf) == 0) {
            return;
          }
          param_1 = iVar3;
          if (((&DAT_00b67d1c)[DAT_0070e488] & 0x40) == 0) {
            return;
          }
        }
      }
      else if (((&DAT_00b67d1c)[DAT_0070e488] & 0x40) != 0) {
        restore_world_building_draw_00488c20(0x00bca350u,param_1,DAT_0070e458 + 2 + param_2,param_3 - DAT_0070e448,1u);
      }
    }
  }
  iVar3 = DAT_0070e45c * uVar5;
  param_3 = DAT_0070e44c * uVar5 + -2 + param_3;
  iVar6 = 0;
  do {
    iVar4 = iVar6;
    if (iVar6 == 0xc) {
      iVar4 = 9;
    }
    if ((*(uint *)(&DAT_00799ad8 + param_1 * 0x108) & 1 << ((byte)iVar6 & 0x1f)) != 0) {
      FUN_0053e590(DAT_00f5644c + iVar4,iVar3 + 0x12 + param_2,param_3);
      param_3 = param_3 + -0xe;
    }
    iVar6 = iVar6 + 1;
  } while (iVar6 < 0xe);
  return;
}
