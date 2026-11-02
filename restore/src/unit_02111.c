#include "restore_types.h"
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_005e4ef8 (*(undefined4 *)0x005e4ef8u)
#define DAT_005e6578 (*(undefined4 *)0x005e6578u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd620 (*(undefined4 *)0x00bcd620u)
#define DAT_00bcd642 (*(undefined4 *)0x00bcd642u)
#define DAT_00bcd644 (*(undefined4 *)0x00bcd644u)
#define DAT_00bcd646 (*(undefined4 *)0x00bcd646u)
#define DAT_00bcd648 (*(undefined4 *)0x00bcd648u)
#define DAT_00bcd64a (*(undefined4 *)0x00bcd64au)
#define DAT_00bcd64c (*(undefined4 *)0x00bcd64cu)
#define DAT_00bcd718 (*(undefined4 *)0x00bcd718u)
extern undefined1 FUN_0051cd80();
extern longlong FUN_0056133c();

void FUN_004f02f0(int param_1,int param_2,undefined2 param_3,char param_4)

{
  short sVar1;
  short sVar2;
  short sVar3;
  undefined1 uVar4;
  short sVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  
  iVar8 = param_1 * 0x184;
  sVar1 = (&DAT_00bcd642)[param_1 * 0xc2];
  (&DAT_00bcd642)[param_1 * 0xc2] = *(undefined2 *)(&DAT_00bcd646 + iVar8);
  (&DAT_00bcd644)[param_1 * 0xc2] = *(undefined2 *)(&DAT_00bcd648 + iVar8);
  iVar9 = param_2 * 0x108;
  *(short *)(&DAT_00bcd646 + iVar8) = (short)param_2;
  *(undefined2 *)(&DAT_00bcd648 + iVar8) = param_3;
  sVar2 = *(short *)((int)&DAT_007999dc + iVar9 + 2);
  sVar3 = *(short *)(&DAT_007999e0 + iVar9);
  *(short *)(&DAT_00bcd61c + iVar8) = sVar2;
  *(short *)(&DAT_00bcd61e + iVar8) = sVar3;
  *(undefined4 *)(&DAT_00bcd620 + iVar8) = *(undefined4 *)(&DAT_007999e4 + iVar9);
  (&DAT_00bcd64a)[param_1 * 0xc2] = 0;
  sVar5 = FUN_0056133c();
  (&DAT_00bcd64c)[param_1 * 0xc2] = sVar5 * 0xf;
  uVar4 = FUN_0051cd80((int)*(short *)((int)&DAT_007999dc + sVar1 * 0x108 + 2),
                       (int)*(short *)(&DAT_007999e0 + sVar1 * 0x108),(int)sVar2,(int)sVar3);
  (&DAT_00bcd609)[iVar8] = uVar4;
  if (param_4 == '\0') {
    return;
  }
  if (DAT_005e4ef4 == 5) {
    iVar9 = *(int *)(&DAT_005e6578 +
                    ((int)(short)(&DAT_007999ea)[param_2 * 0x84] +
                    (uint)*(byte *)(&DAT_00799a2c + param_2 * 0x84) * 0x24) * 8);
  }
  else {
    iVar9 = *(int *)(&DAT_005e4ef8 +
                    ((uint)*(byte *)(&DAT_00799a2c + param_2 * 0x84) * 0x90 +
                    (int)(short)(&DAT_007999ea)[param_2 * 0x84]) * 8);
  }
  iVar6 = DAT_00bca2e0 / 2;
  if (iVar9 < 4) {
    iVar7 = iVar9 + iVar6;
    if (iVar9 + iVar6 < 4) goto LAB_004f047a;
  }
  else {
    iVar7 = iVar9;
    if ((7 < iVar9) || (iVar7 = iVar9 + iVar6, iVar7 < 8)) goto LAB_004f047a;
  }
  iVar7 = iVar9 + iVar6 + -4;
LAB_004f047a: ;
  switch((int)(iVar7)) {
  case 0: ;
  case 1: ;
  case 4: ;
  case 7: ;
    *(undefined2 *)(&DAT_00bcd718 + iVar8) = 3;
    return;
default: ;
    *(undefined2 *)(&DAT_00bcd718 + iVar8) = 2;
    return;
  }
}
