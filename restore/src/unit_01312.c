#include "restore_types.h"
#define DAT_005dc5fc (*(undefined4 *)0x005dc5fcu)
#define DAT_005dc614 (*(undefined4 *)0x005dc614u)
#define DAT_005dc630 (*(undefined4 *)0x005dc630u)
#define DAT_005dc6ac (*(undefined4 *)0x005dc6acu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd642 (*(undefined4 *)0x00bcd642u)
#define DAT_00bcd644 (*(undefined4 *)0x00bcd644u)
#define DAT_00bcd651 (*(undefined4 *)0x00bcd651u)
#define DAT_00bcd754 (*(undefined4 *)0x00bcd754u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_004b4220();
extern int FUN_004b4800();
extern void FUN_0053b090();

void FUN_0048c0e0(int param_1,int param_2,int param_3,char param_4)

{
  byte bVar1;
  short sVar2;
  short sVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  byte *pbVar7;
  short *psVar8;
  int iVar9;
  
  iVar4 = *(int *)(&DAT_005dc5fc + param_3 * 4);
  iVar5 = *(int *)(&DAT_005dc614 + param_3 * 4);
  param_3 = (-(uint)(param_4 != '\0') & 0xc) + 4;
  if (param_3 != 0) {
    pbVar7 = &DAT_005dc6ac;
    psVar8 = &DAT_005dc630;
    do {
      iVar6 = FUN_004b4800(DAT_00eaac28,6,iVar4 + param_1,param_2 + iVar4,0);
      if (iVar6 != 0) {
        sVar2 = psVar8[-2];
        sVar3 = *psVar8;
        iVar9 = iVar6 * 0x184;
        (&DAT_00bcd642)[iVar6 * 0xc2] = (&DAT_00bcd642)[iVar6 * 0xc2] + (short)iVar5;
        (&DAT_00bcd644)[iVar6 * 0xc2] = (&DAT_00bcd644)[iVar6 * 0xc2] + (short)iVar5;
        *(short *)(&DAT_00bcd61c + iVar9) = *(short *)(&DAT_00bcd61c + iVar9) + sVar2;
        *(short *)(&DAT_00bcd61e + iVar9) = *(short *)(&DAT_00bcd61e + iVar9) + sVar3;
        FUN_004b4220(iVar6,(int)(short)(&DAT_00bcd642)[iVar6 * 0xc2],
                     (int)(short)(&DAT_00bcd644)[iVar6 * 0xc2],
                     iVar5 + *(short *)(&DAT_00bcd61c + iVar9) * 0xf,
                     iVar5 + *(short *)(&DAT_00bcd61e + iVar9) * 0xf,0);
        bVar1 = *pbVar7;
        (&DAT_00bcd651)[iVar9] = bVar1;
        *(uint *)(&DAT_00bcd754 + iVar9) = (uint)(bVar1 < 3);
      }
      psVar8 = psVar8 + 4;
      pbVar7 = pbVar7 + 4;
      param_3 = param_3 + -1;
    } while (param_3 != 0);
  }
  if (param_4 != '\0') {
    FUN_0053b090(0x11);
  }
  return;
}
