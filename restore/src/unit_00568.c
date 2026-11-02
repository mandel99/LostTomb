#include "restore_types.h"
#define DAT_005cfcf0 (*(undefined4 *)0x005cfcf0u)
#define DAT_005d1184 (*(undefined *)0x005d1184u)
#define DAT_005d1188 (*(undefined *)0x005d1188u)
#define DAT_0070e120 (*(undefined *)0x0070e120u)
#define DAT_0070e13c (*(undefined *)0x0070e13cu)
#define DAT_0070e44c (*(undefined *)0x0070e44cu)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799a82 (*(undefined4 *)0x00799a82u)
#define DAT_00799a9c (*(undefined4 *)0x00799a9cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bc96e0 (*(undefined *)0x00bc96e0u)
#define DAT_00bc96e4 (*(undefined *)0x00bc96e4u)
#define DAT_00bc9800 (*(undefined *)0x00bc9800u)
#define DAT_00bca300 (*(undefined *)0x00bca300u)
#define DAT_00bca304 (*(undefined *)0x00bca304u)
#define DAT_00bca308 (*(undefined *)0x00bca308u)
#define DAT_00bca310 (*(undefined *)0x00bca310u)
#define DAT_00bca314 (*(char *)0x00bca314u)
#define DAT_00f55bb4 (*(undefined4 *)0x00f55bb4u)
#define DAT_00f56be4 (*(undefined *)0x00f56be4u)
extern int FUN_0043d010();
extern uint FUN_0043d030();
extern void FUN_0043dd70();
extern int FUN_0047cbd0();
extern int FUN_0047cc70();
extern void FUN_0047f3e0();
extern undefined4 FUN_00486a30();
extern undefined4 FUN_00486cf0();
extern void FUN_004f0ba0();
extern int FUN_0053e590();
extern undefined4 FUN_0053f250();
extern int FUN_0053f4f0();

void FUN_0043d300(void)

{
  byte bVar1;
  short sVar2;
  bool bVar3;
  char cVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int *piVar9;
  int iVar10;
  undefined4 *puVar11;
  int local_fb8;
  int local_fb4;
  int local_fb0 [3];
  undefined4 local_fa4;
  undefined4 local_fa0 [1000];
  
  DAT_00bca314 = 0;
  bVar3 = false;
  FUN_004f0ba0(DAT_00bca304,DAT_00bca308,&local_fb4,&local_fb8);
  local_fb8 = local_fb8 + DAT_0070e44c;
  if (DAT_00bca310 == 0) {
    FUN_0047f3e0(DAT_00bca304,DAT_00bca308,0);
    iVar6 = local_fb4;
    iVar7 = DAT_00bc96e0;
    if (DAT_00bc96e0 < 0) {
      iVar7 = DAT_00bc96e0 + 8;
    }
    iVar10 = local_fb8 - DAT_0070e44c;
    DAT_0070e13c = 0;
    local_fb0[0] = FUN_0047cc70(DAT_00bca304,DAT_00bca308,1,0);
    sVar2 = *(short *)(&DAT_00b4e6fc + DAT_00bca300 * 2);
    iVar5 = FUN_00486cf0(DAT_00bca300);
    if ((iVar5 != 0) && ((&DAT_00799a9c)[sVar2 * 0x42] == -1)) {
      bVar3 = true;
      FUN_0043d030(iVar7,DAT_00bca304,DAT_00bca308,iVar6,iVar10,DAT_00bc96e4);
      DAT_0070e120 = 0x1fe3;
      iVar8 = (&DAT_00f55bb4)[DAT_005d1184 * 2] + DAT_005d1188;
      iVar5 = FUN_0043d010(iVar7);
      iVar8 = iVar8 + iVar5;
      FUN_0053e590(iVar8,local_fb4,local_fb8 - DAT_0070e44c);
      FUN_0053f250(iVar8,local_fb4,local_fb8 - DAT_0070e44c);
      DAT_0070e120 = 0;
    }
    if ((local_fb0[0] == 0) &&
       (cVar4 = FUN_0043d030(iVar7,DAT_00bca304,DAT_00bca308,iVar6,iVar10,DAT_00bc96e4),
       cVar4 != '\0')) {
      DAT_00bca314 = 1;
      FUN_0043dd70(0,0);
      DAT_0070e120 = 0;
      return;
    }
    if (!bVar3) {
      DAT_00bca314 = 0;
      piVar9 = &DAT_005cfcf0;
      do {
        FUN_0053f4f0(DAT_00f56be4,*piVar9 + local_fb4,(piVar9[1] - DAT_0070e44c) + local_fb8,
                     0xfffff863);
        piVar9 = piVar9 + 2;
      } while ((int)piVar9 < 0x5cfd10);
      return;
    }
  }
  else {
    puVar11 = local_fa0;
    for (iVar6 = 0xfa; iVar6 != 0; iVar6 = iVar6 + -1) {
      *puVar11 = 0xffffffff;
      puVar11 = puVar11 + 1;
    }
    iVar6 = FUN_0047cbd0(DAT_00bca304,DAT_00bca308,0);
    iVar7 = DAT_00bc9800 * 0x108;
    local_fb0[1] = 0;
    local_fb0[2] = (int)(byte)(&DAT_007999db)[iVar7];
    bVar1 = *(byte *)(&DAT_00799a82 + DAT_00bc9800 * 0x84);
    iVar10 = (int)*(short *)((int)&DAT_007999dc + iVar7 + 2);
    iVar5 = (int)*(short *)(&DAT_007999e0 + iVar7);
    iVar7 = FUN_00486cf0(DAT_00bca300);
    if (((iVar7 != 0) && (*(short *)(&DAT_00b4e6fc + DAT_00bca300 * 2) != DAT_00bc9800)) &&
       ((&DAT_00799a9c)[*(short *)(&DAT_00b4e6fc + DAT_00bca300 * 2) * 0x42] == -1)) {
      DAT_0070e120 = 0x1fe3;
      iVar7 = (&DAT_00f55bb4)[DAT_005d1184 * 2] + DAT_005d1188;
      iVar6 = FUN_0043d010(iVar6 * 2);
      iVar7 = iVar7 + iVar6;
      FUN_0053e590(iVar7,local_fb4,local_fb8 - DAT_0070e44c);
      FUN_0053f250(iVar7,local_fb4,local_fb8 - DAT_0070e44c);
      DAT_0070e120 = 0;
      return;
    }
    FUN_004f0ba0(iVar10,iVar5,&local_fa4,local_fb0);
    FUN_0043d030((uint)bVar1 * 2,iVar10,iVar5,local_fa4,local_fb0[0],DAT_00bc96e4);
    iVar6 = FUN_00486a30(DAT_00bca304,DAT_00bca308,iVar10,iVar5,local_fb0[2],iVar6 * 2,
                         (uint)bVar1 * 2 + 4 & 0x80000007,local_fa0,local_fb0 + 1);
    if (iVar6 == 1) {
      DAT_00bca314 = 1;
      FUN_0043dd70(0,0);
      return;
    }
    DAT_00bca314 = 0;
    piVar9 = &DAT_005cfcf0;
    do {
      FUN_0053f4f0(DAT_00f56be4,*piVar9 + local_fb4,(piVar9[1] - DAT_0070e44c) + local_fb8,
                   0xfffff863);
      piVar9 = piVar9 + 2;
    } while ((int)piVar9 < 0x5cfd10);
  }
  return;
}
