#include "restore_types.h"
#define DAT_0056f840 (*(undefined4 *)0x0056f840u)
#define DAT_0056f880 (*(undefined4 *)0x0056f880u)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_005e4ef8 (*(undefined4 *)0x005e4ef8u)
#define DAT_005e6578 (*(undefined4 *)0x005e6578u)
#define DAT_005e68d8 (*(undefined4 *)0x005e68d8u)
#define DAT_005f5618 (*(undefined4 *)0x005f5618u)
#define DAT_005f561c (*(undefined4 *)0x005f561cu)
#define DAT_005f57c8 (*(undefined4 *)0x005f57c8u)
#define DAT_005f57cc (*(undefined4 *)0x005f57ccu)
#define DAT_005f5b28 (*(undefined4 *)0x005f5b28u)
#define DAT_005f5b2c (*(undefined4 *)0x005f5b2cu)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a34 (*(undefined4 *)0x00799a34u)
#define DAT_00799a36 (*(undefined4 *)0x00799a36u)
#define DAT_00799a38 (*(undefined4 *)0x00799a38u)
#define DAT_00799a3a (*(undefined4 *)0x00799a3au)
#define DAT_00799a40 (*(undefined4 *)0x00799a40u)
#define DAT_00799a44 (*(undefined4 *)0x00799a44u)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00f57334 (*(undefined *)0x00f57334u)
#define DAT_00f57364 (*(undefined *)0x00f57364u)
#define DAT_00f5738c (*(undefined *)0x00f5738cu)
extern void FUN_0047bf50();
extern char FUN_004f3f70();
extern void FUN_004f4160();

void FUN_004edf50(int param_1,int param_2,int param_3)

{
  byte bVar1;
  byte bVar2;
  undefined2 uVar3;
  short sVar4;
  short sVar5;
  bool bVar6;
  uint uVar7;
  int iVar8;
  int iVar9;
  uint uVar10;
  uint uVar11;
  int iVar12;
  int iVar13;
  int *piVar14;
  undefined4 uVar15;
  uint local_4;
  
  uVar10 = 0;
  iVar13 = param_1 * 0x108;
  *(undefined4 *)(&DAT_00799a44 + param_1 * 0x84) = 0;
  if (1 < *(byte *)((int)&DAT_00799a2c + iVar13 + 1)) {
    FUN_004f4160(param_1,param_2,param_3);
    return;
  }
  iVar12 = (int)(short)(&DAT_007999ea)[param_1 * 0x84];
  if (DAT_005e4ef4 == 5) {
    uVar7 = *(uint *)(&DAT_005e6578 + (iVar12 + param_3 * 0x24) * 8);
  }
  else {
    uVar7 = *(uint *)(&DAT_005e4ef8 + (param_3 * 0x90 + iVar12) * 8);
  }
  if ((int)uVar7 < 4) {
    uVar7 = uVar7 + param_2;
    if (3 < (int)uVar7) {
      uVar7 = uVar7 - 4;
    }
  }
  else if ((int)uVar7 < 8) {
    uVar7 = uVar7 + param_2;
    if (7 < (int)uVar7) {
      uVar7 = uVar7 - 4;
    }
  }
  else if (uVar7 == 9) {
    uVar7 = param_2 + 9;
  }
  if (DAT_005e4ef4 == 5) {
    local_4 = 0;
    uVar11 = uVar10;
  }
  else {
    uVar11 = uVar7;
    if (uVar7 == 5) {
      local_4 = DAT_00f57334 + 0x6d;
    }
    else if (uVar7 == 6) {
      if (DAT_005e4ef4 == 3) {
        local_4 = DAT_00f57334 + 0x70;
      }
      else {
        local_4 = DAT_00f57334 + 0x6e;
      }
    }
    else {
      local_4 = uVar10;
      if (DAT_005e4ef4 == 3) {
        if (uVar7 == 1) {
          local_4 = DAT_00f57334 + 0x73;
        }
        else if (uVar7 == 2) {
          local_4 = DAT_00f57334 + 0x77;
        }
        else if (uVar7 == 3) {
          local_4 = DAT_00f57334 + 0x7b;
        }
      }
    }
  }
  bVar6 = false;
  if ((*(char *)((int)&DAT_00799a2c + iVar13 + 1) == '\0') || (DAT_005e4ef4 == 5)) {
    bVar2 = *(byte *)(&DAT_00799a2e + param_1 * 0x84);
    if (bVar2 == 0) {
      if (((short)(&DAT_007999ea)[param_1 * 0x84] < 4) && (DAT_005e4ef4 != 5)) {
        iVar12 = *(int *)(&DAT_0056f840 + (iVar12 + param_2 * 4) * 4) + DAT_00f57364;
      }
      else {
        iVar12 = 0;
        bVar6 = true;
      }
    }
    else {
      bVar1 = *(byte *)((int)&DAT_00799a2e + iVar13 + 1);
      if ((bVar1 < *(byte *)(&DAT_00799a30 + param_1 * 0x42)) || (uVar7 == 8)) {
        iVar12 = DAT_00f57334 + (uint)bVar2 + 0x5f +
                 ((byte)(&DAT_00acf85c)[*(int *)(&DAT_007999e4 + iVar13)] & 1) * 6;
      }
      else if (DAT_005e4ef4 == 1) {
        if ((&DAT_005e68d8)[param_3 * 6 + (uint)bVar1] == '\0') {
LAB_004ee179: ;
          iVar12 = DAT_00f57334 + (uint)bVar2 * 8 + -8 + uVar7;
        }
        else {
          iVar12 = DAT_00f5738c + (uint)bVar2 * 8 + -8 + uVar7;
        }
      }
      else {
        if ((int)uVar7 < 8) goto LAB_004ee179;
        iVar12 = (uVar7 & 1) + 0x6d + DAT_00f57334;
      }
    }
  }
  else if ((DAT_005e4ef4 == 1) &&
          ((&DAT_005e68d8)[param_3 * 6 + (uint)*(byte *)((int)&DAT_00799a2e + iVar13 + 1)] != '\0'))
  {
    iVar12 = DAT_00f5738c + (5 - (uint)*(byte *)(&DAT_00799a2e + param_1 * 0x84)) * 8 + uVar7;
  }
  else {
    iVar12 = DAT_00f57334 + (0xb - (uint)*(byte *)(&DAT_00799a2e + param_1 * 0x84)) * 8 + uVar7;
  }
  bVar2 = *(byte *)((int)&DAT_00799a30 + iVar13 + 1);
  if (((bVar2 == 0) && (*(char *)((int)&DAT_00799a30 + iVar13 + 2) == '\0')) || (local_4 == 0)) {
    *(undefined4 *)(&DAT_00799a40 + param_1 * 0x84) = 0;
    goto LAB_004ee400;
  }
  if (DAT_005e4ef4 == 1) {
    if ((&DAT_005e68d8)[param_3 * 6 + (uint)*(byte *)((int)&DAT_00799a2e + iVar13 + 1)] == '\0') {
LAB_004ee35a: ;
      if (bVar2 != 0) {
        iVar9 = ((int)(bVar2 - 1) % 6 + uVar11 * 6) * 8;
        *(short *)(&DAT_00799a34 + iVar13) =
             *(short *)(&DAT_005f5b28 + uVar11 * 8) + *(short *)(&DAT_005f5618 + iVar9);
        *(short *)(&DAT_00799a36 + iVar13) =
             *(short *)(&DAT_005f5b2c + uVar11 * 8) + *(short *)(&DAT_005f561c + iVar9);
      }
      bVar2 = *(byte *)((int)&DAT_00799a30 + iVar13 + 2);
      if (bVar2 != 0) {
        sVar4 = *(short *)(&DAT_005f5b2c + uVar11 * 8);
        iVar9 = ((int)(bVar2 - 1) % 6 + uVar11 * 6) * 8;
        sVar5 = *(short *)(&DAT_005f561c + iVar9);
        *(short *)(&DAT_00799a38 + iVar13) =
             *(short *)(&DAT_005f5b28 + uVar11 * 8) + *(short *)(&DAT_005f5618 + iVar9);
        *(short *)(&DAT_00799a3a + iVar13) = sVar4 + sVar5;
      }
    }
    else {
      if (bVar2 != 0) {
        iVar8 = ((int)((uint)bVar2 * 2 + -2) % 6 + uVar11 * 6) * 8;
        iVar9 = *(int *)(&DAT_005f561c + iVar8);
        *(short *)(&DAT_00799a34 + iVar13) =
             *(short *)(&DAT_005f5b28 + uVar11 * 8) + *(short *)(&DAT_005f5618 + iVar8);
        *(short *)(&DAT_00799a36 + iVar13) =
             (short)(iVar9 / 2) + *(short *)(&DAT_005f5b2c + uVar11 * 8);
      }
      bVar2 = *(byte *)((int)&DAT_00799a30 + iVar13 + 2);
      if (bVar2 != 0) {
        iVar8 = ((int)((uint)bVar2 * 2 + -2) % 6 + uVar11 * 6) * 8;
        iVar9 = *(int *)(&DAT_005f561c + iVar8);
        *(short *)(&DAT_00799a38 + iVar13) =
             *(short *)(&DAT_005f5b28 + uVar11 * 8) + *(short *)(&DAT_005f5618 + iVar8);
        *(short *)(&DAT_00799a3a + iVar13) =
             (short)(iVar9 / 2) + *(short *)(&DAT_005f5b2c + uVar11 * 8);
        *(uint *)(&DAT_00799a40 + param_1 * 0x84) = local_4;
        goto LAB_004ee400;
      }
    }
  }
  else {
    if (DAT_005e4ef4 != 3) goto LAB_004ee35a;
    if (bVar2 != 0) {
      iVar9 = (((2 < (int)(bVar2 - 1) % 6) - 1 & 0xfffffffd) + 3 + uVar11 * 6) * 0x10;
      uVar3 = *(undefined2 *)(&DAT_005f57cc + iVar9);
      *(undefined2 *)(&DAT_00799a34 + iVar13) = *(undefined2 *)(&DAT_005f57c8 + iVar9);
      *(undefined2 *)(&DAT_00799a36 + iVar13) = uVar3;
    }
    bVar2 = *(byte *)((int)&DAT_00799a30 + iVar13 + 2);
    if (bVar2 != 0) {
      iVar9 = (((2 < (int)(bVar2 - 1) % 6) - 1 & 0xfffffffd) + 3 + uVar11 * 6) * 0x10;
      uVar3 = *(undefined2 *)(&DAT_005f57cc + iVar9);
      *(undefined2 *)(&DAT_00799a38 + iVar13) = *(undefined2 *)(&DAT_005f57c8 + iVar9);
      *(undefined2 *)(&DAT_00799a3a + iVar13) = uVar3;
      *(uint *)(&DAT_00799a40 + param_1 * 0x84) = local_4;
      goto LAB_004ee400;
    }
  }
  *(uint *)(&DAT_00799a40 + param_1 * 0x84) = local_4;
LAB_004ee400: ;
  if (!bVar6) {
    FUN_0047bf50(param_1,(int)*(short *)((int)&DAT_007999dc + iVar13 + 2),
                 (int)*(short *)(&DAT_007999e0 + iVar13),(&DAT_007999db)[iVar13],
                 (&DAT_007999db)[iVar13],iVar12,8);
    return;
  }
  piVar14 = &DAT_0056f880;
  do {
    iVar12 = *(int *)(&DAT_007999e4 + iVar13) + *piVar14;
    uVar10 = (&DAT_00af598c)[iVar12];
    *(short *)(&DAT_00b4e6fc + iVar12 * 2) = (short)param_1;
    (&DAT_00af598c)[iVar12] = uVar10 & 0xefffffff | 8;
    if (DAT_005e4ef4 == 5) {
      uVar15 = 5;
    }
    else {
      uVar15 = 10;
    }
    FUN_004f3f70(param_1,iVar12,uVar15,1);
    piVar14 = piVar14 + 1;
  } while ((int)piVar14 < 0x56f890);
  return;
}
