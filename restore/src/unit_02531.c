#include "restore_types.h"
#define DAT_005e06e0 (*(undefined4 *)0x005e06e0u)
#define DAT_006018f8 (*(undefined *)0x006018f8u)
#define DAT_006018fc (*(undefined *)0x006018fcu)
#define DAT_0070e120 (*(undefined *)0x0070e120u)
#define DAT_00d3b530 (*(undefined1 *)0x00d3b530u)
#define DAT_00d3b531 (*(undefined1 *)0x00d3b531u)
#define DAT_00d3b538 (*(undefined *)0x00d3b538u)
#define DAT_00d3b53c (*(undefined *)0x00d3b53cu)
#define DAT_00d3b540 (*(undefined *)0x00d3b540u)
#define DAT_00d3b544 (*(undefined *)0x00d3b544u)
#define DAT_00d3b54c (*(undefined *)0x00d3b54cu)
#define DAT_00d3b568 (*(undefined *)0x00d3b568u)
#define DAT_00d3b56c (*(undefined *)0x00d3b56cu)
#define DAT_00d3b570 (*(undefined *)0x00d3b570u)
#define DAT_00d3b574 (*(undefined *)0x00d3b574u)
#define DAT_00d3b578 (*(undefined *)0x00d3b578u)
#define DAT_00d3b57c (*(undefined *)0x00d3b57cu)
#define DAT_00d3b580 (*(undefined *)0x00d3b580u)
#define DAT_00d3b5b4 (*(undefined1 *)0x00d3b5b4u)
#define DAT_00d3b5b8 (*(undefined *)0x00d3b5b8u)
#define DAT_00d3b5bc (*(undefined *)0x00d3b5bcu)
#define DAT_00d3b5be (*(undefined *)0x00d3b5beu)
#define DAT_00d3b5d4 (*(undefined *)0x00d3b5d4u)
#define DAT_00d3b5dc (*(undefined *)0x00d3b5dcu)
#define DAT_00d3b5e0 (*(undefined *)0x00d3b5e0u)
#define DAT_00e38aa8 (*(undefined4 *)0x00e38aa8u)
#define DAT_00e38d9c (*(undefined *)0x00e38d9cu)
#define DAT_00e38dac (*(undefined *)0x00e38dacu)
#define DAT_00e38db0 (*(undefined *)0x00e38db0u)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00e92e1c (*(uint32_t *)0x00e92e1cu)
#define DAT_00f56db4 (*(undefined *)0x00f56db4u)
extern void FUN_0040fb60();
extern void FUN_0040fc40();
extern void FUN_0040fcf0();
extern void FUN_0040fdd0();
extern void FUN_0040fed0();
extern void FUN_0040ff50();
extern void FUN_00410090();
extern void FUN_00410700();
extern void FUN_004109d0();
extern void FUN_00410a70();
extern void FUN_00410b10();
extern undefined4 FUN_00410de0();
extern undefined4 FUN_00410e80();
extern void FUN_00410f70();
extern void FUN_00410fb0();
extern void FUN_004114a0();
extern void FUN_00411fd0();
extern undefined4 FUN_00427a20();
extern undefined4 FUN_0042a160();
extern undefined4 FUN_0042b440();
extern undefined4 FUN_0042c740();
extern undefined4 FUN_0042c9a0();
extern undefined4 FUN_0042cee0();
extern undefined4 FUN_0042d810();
extern undefined4 FUN_0042e680();
extern void FUN_004cdad0();
extern void FUN_004cdb80();
extern void FUN_004cdeb0();
extern void FUN_004cdf20();
extern void FUN_004ce310();
extern int FUN_004ce480();
extern void FUN_0051f780();
extern void FUN_0053f790();
extern int FUN_0053f7d0();

int FUN_0053fa40(int param_1,undefined4 param_2,undefined4 param_3)

{
  int *piVar1;
  int *piVar2;
  char cVar3;
  int iVar4;
  int in_ECX;
  uint uVar5;
  uint uVar6;
  undefined2 extraout_var;
  undefined2 extraout_var_00;
  undefined2 extraout_var_01;
  undefined2 uVar7;
  int iVar8;
  char *pcVar9;
  undefined4 *puVar10;
  char *pcVar11;
  char *pcVar12;
  undefined4 local_104;
  undefined2 local_100;
  undefined4 local_fe [63];
  
  DAT_00d3b57c = param_2;
  DAT_00d3b580 = param_1;
  DAT_00d3b578 = param_3;
  iVar8 = param_1;
  if (*(int *)(in_ECX + 0x30f5cc) <= param_1) {
    iVar8 = 0;
  }
  piVar2 = (int *)(iVar8 * 0x40 + 0x180e48 + in_ECX);
  if (param_1 < 1) {
    return 0;
  }
  if (0x639b < param_1) {
    return 0;
  }
  if (piVar2 == (int *)0x0) {
    return 0;
  }
  DAT_00d3b5b8 = *piVar2;
  if (DAT_00d3b5b8 < 1) {
    return 0;
  }
  DAT_00d3b5be = (short)piVar2[5];
  DAT_00d3b5bc = *(undefined2 *)((int)piVar2 + 0x16);
  DAT_00d3b5b4 = *(char *)((int)piVar2 + 0x32);
  DAT_00d3b5dc = param_3;
  iVar8 = (int)DAT_00d3b5be;
  DAT_00d3b5e0 = param_2;
  DAT_00d3b544 = iVar8;
  if (((char)piVar2[0xd] != '\0') && (iVar8 <= DAT_00e38d9c)) {
    local_104 = DAT_006018f8;
    local_100 = DAT_006018fc;
    puVar10 = local_fe;
    for (iVar8 = 0x3f; iVar8 != 0; iVar8 = iVar8 + -1) {
      *puVar10 = 0;
      puVar10 = puVar10 + 1;
    }
    iVar8 = *(int *)(in_ECX + 0x30f5cc);
    *(undefined2 *)puVar10 = 0;
    iVar4 = param_1;
    if ((iVar8 <= param_1) && (iVar4 = 0, iVar8 <= param_1)) {
      param_1 = 0;
    }
    uVar5 = 0xffffffff;
    pcVar9 = (char *)(in_ECX + *(int *)(param_1 * 0x40 + 0x180e54 + in_ECX) * 0xcc3c + 0x2044 +
                     (uint)*(byte *)((iVar4 + 0x603a) * 0x40 + in_ECX) * 0x104);
    do {
      pcVar12 = pcVar9;
      if (uVar5 == 0) break;
      uVar5 = uVar5 - 1;
      pcVar12 = pcVar9 + 1;
      cVar3 = *pcVar9;
      pcVar9 = pcVar12;
    } while (cVar3 != '\0');
    uVar5 = ~uVar5;
    iVar8 = -1;
    pcVar9 = (char *)&local_104;
    do {
      pcVar11 = pcVar9;
      if (iVar8 == 0) break;
      iVar8 = iVar8 + -1;
      pcVar11 = pcVar9 + 1;
      cVar3 = *pcVar9;
      pcVar9 = pcVar11;
    } while (cVar3 != '\0');
    pcVar9 = pcVar12 + -uVar5;
    pcVar12 = pcVar11 + -1;
    for (uVar6 = uVar5 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
      *(undefined4 *)pcVar12 = *(undefined4 *)pcVar9;
      pcVar9 = pcVar9 + 4;
      pcVar12 = pcVar12 + 4;
    }
    for (uVar5 = uVar5 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
      *pcVar12 = *pcVar9;
      pcVar9 = pcVar9 + 1;
      pcVar12 = pcVar12 + 1;
    }
    FUN_004ce310(&local_104,&DAT_005e06e0);
    piVar1 = (int *)(in_ECX + 0x30f5c0);
    FUN_0053f790(piVar1,0);
    iVar8 = DAT_00e92e1c;
    *piVar1 = DAT_00e92e1c;
    iVar8 = FUN_004ce480(&local_104,iVar8,piVar2[1],*piVar2 + -1);
    if (iVar8 == 0) {
      return 0;
    }
    if (*(char *)((int)piVar2 + 0x33) == '\0') {
      FUN_004cdf20(*piVar1,piVar2[1]);
    }
    else {
      FUN_004cdeb0(*piVar1,piVar2[1]);
    }
    FUN_004cdad0();
    FUN_004cdb80();
    iVar8 = (int)DAT_00d3b5be;
    if (DAT_00d3b530 == '\x05') {
      return 0;
    }
    if (DAT_00d3b5b4 != '\0') {
      DAT_00d3b5b8 = (iVar8 * DAT_00d3b538 + DAT_00d3b54c) * 2;
      if (DAT_00d3b531 == '\x01') {
        FUN_0040ff50(*piVar1 + DAT_00d3b5b8);
        return iVar8;
      }
      if (DAT_00d3b531 == '\x02') {
        FUN_0040ff50(*piVar1 + DAT_00d3b5b8);
        return iVar8;
      }
      FUN_0040fed0(*piVar1 + DAT_00d3b5b8);
      return iVar8;
    }
    if (*(char *)((int)piVar2 + 0x33) == '\0') {
      DAT_00d3b5b8 = (iVar8 * DAT_00d3b538 + DAT_00d3b54c) * 2;
      if (DAT_00d3b531 == '\x01') {
        FUN_0040ff50(*piVar1 + DAT_00d3b5b8);
        return iVar8;
      }
      if (DAT_00d3b531 == '\x02') {
        FUN_0040ff50(*piVar1 + DAT_00d3b5b8);
        return iVar8;
      }
      FUN_0040fed0(*piVar1 + DAT_00d3b5b8);
      return iVar8;
    }
    if (DAT_00d3b531 == '\x01') {
      FUN_0040fdd0(*piVar1);
      return iVar8;
    }
    if (DAT_00d3b531 == '\x02') {
      FUN_0040fcf0(*piVar1);
      return iVar8;
    }
    FUN_0040fc40(*piVar1);
    return iVar8;
  }
  if ((char)piVar2[0xd] != '\0') {
    DAT_00d3b574 = DAT_00e38db0;
    DAT_00d3b544 = DAT_00e38db0;
    DAT_00d3b568 = (0x300 - DAT_00e38dac) / 2;
    DAT_00d3b56c = (0x400 - DAT_00e38db0) / 2;
    iVar4 = iVar8 * DAT_00d3b568 + DAT_00d3b56c;
    DAT_00d3b570 = DAT_00e38dac;
    DAT_00d3b5d4 = DAT_00e38dac;
    iVar8 = param_1;
    if ((*(int *)(in_ECX + 0x30f5cc) <= param_1) &&
       (iVar8 = 0, *(int *)(in_ECX + 0x30f5cc) <= param_1)) {
      param_1 = 0;
    }
    uVar5 = 0xffffffff;
    pcVar9 = (char *)(in_ECX + *(int *)(param_1 * 0x40 + 0x180e54 + in_ECX) * 0xcc3c + 0x2044 +
                     (uint)*(byte *)((iVar8 + 0x603a) * 0x40 + in_ECX) * 0x104);
    do {
      pcVar12 = pcVar9;
      if (uVar5 == 0) break;
      uVar5 = uVar5 - 1;
      pcVar12 = pcVar9 + 1;
      cVar3 = *pcVar9;
      pcVar9 = pcVar12;
    } while (cVar3 != '\0');
    uVar5 = ~uVar5;
    pcVar9 = pcVar12 + -uVar5;
    pcVar12 = (char *)&DAT_00e38aa8;
    for (uVar6 = uVar5 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
      *(undefined4 *)pcVar12 = *(undefined4 *)pcVar9;
      pcVar9 = pcVar9 + 4;
      pcVar12 = pcVar12 + 4;
    }
    for (uVar5 = uVar5 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
      *pcVar12 = *pcVar9;
      pcVar9 = pcVar9 + 1;
      pcVar12 = pcVar12 + 1;
    }
    piVar2 = (int *)(in_ECX + 0x30f5c0);
    FUN_0053f790(piVar2,0);
    *piVar2 = DAT_00e92e1c;
    DAT_00d3b5dc = 0;
    DAT_00d3b5e0 = 0;
    FUN_0053f7d0(&DAT_00e38aa8,*piVar2,DAT_00d3b544 * 2,DAT_00d3b5d4,iVar4 * 2);
    DAT_00d3b540 = 0;
    DAT_00d3b53c = 0;
    FUN_0040fb60(*piVar2);
    return DAT_00d3b544;
  }
  if (((DAT_00e38dda == '\x15') || (DAT_00e38dda == '\x14')) && (param_1 == DAT_00f56db4)) {
    DAT_00d3b5d4 = DAT_00d3b570;
    DAT_00d3b544 = DAT_00d3b574;
    DAT_00d3b53c = (DAT_00e38d9c - DAT_00d3b574) * 2;
    DAT_00d3b540 = (iVar8 - DAT_00d3b574) * 2;
    FUN_0040fb60(DAT_00d3b5b8 + (iVar8 * DAT_00d3b568 + DAT_00d3b56c) * 2);
    return DAT_00d3b544;
  }
  if (DAT_00d3b5b4 == '\x1e') {
    if (DAT_00d3b5be == 0x3a) {
      DAT_00d3b5bc = 0x1e;
    }
    else if (DAT_00d3b5be == 0x1a) {
      DAT_00d3b5bc = 0xe;
    }
    else if (DAT_00d3b5be == 10) {
      DAT_00d3b5bc = 6;
    }
    else {
      if (DAT_00d3b5be == 0x76) {
        iVar8 = FUN_0042c740();
        return iVar8;
      }
      if (DAT_00d3b5be == 0xb2) {
        iVar8 = FUN_0042c9a0();
        return iVar8;
      }
      if (DAT_00d3b5be == 0xee) {
        iVar8 = FUN_0042cee0();
        return iVar8;
      }
      if (DAT_00d3b5be == 0x12a) {
        iVar8 = FUN_0042d810();
        return iVar8;
      }
      if (DAT_00d3b5be == 0x166) {
        iVar8 = FUN_0042e680();
        return iVar8;
      }
    }
  }
  FUN_004cdad0();
  FUN_004cdb80();
  if (DAT_00d3b530 == '\x05') {
    return 0;
  }
  if (DAT_00d3b5b4 == '\0') {
    if (*(char *)((int)piVar2 + 0x33) != '\0') {
      if (DAT_00d3b531 == '\x01') {
        FUN_0040fdd0(DAT_00d3b5b8);
        uVar7 = extraout_var;
      }
      else if (DAT_00d3b531 == '\x02') {
        FUN_0040fcf0(DAT_00d3b5b8);
        uVar7 = extraout_var_00;
      }
      else {
        FUN_0040fc40(DAT_00d3b5b8);
        uVar7 = extraout_var_01;
      }
      if (DAT_0070e120 == 0) {
        return DAT_00d3b544;
      }
      if (DAT_00d3b531 == '\x01') {
        FUN_004109d0(DAT_00d3b5b8,CONCAT22(uVar7,DAT_0070e120));
        return DAT_00d3b544;
      }
      if (DAT_00d3b531 == '\x02') {
        FUN_00410a70(DAT_00d3b5b8);
        return DAT_00d3b544;
      }
      FUN_00410700(DAT_00d3b5b8,CONCAT22(uVar7,DAT_0070e120));
      return DAT_00d3b544;
    }
    DAT_00d3b5b8 = DAT_00d3b5b8 + (DAT_00d3b5be * DAT_00d3b538 + DAT_00d3b54c) * 2;
    if (DAT_00d3b531 == '\x01') {
      FUN_0040ff50(DAT_00d3b5b8);
      return DAT_00d3b544;
    }
  }
  else {
    if (DAT_00d3b5b4 == '\x1e') {
      if (DAT_00d3b531 == '\x01') {
        if (DAT_00d3b5be == 0x3a) {
          FUN_0042a160(DAT_00d3b5b8,DAT_00d3b530);
          return DAT_00d3b544;
        }
        if (DAT_00d3b5be == 0x1a) {
          FUN_0051f780(DAT_00d3b5b8,DAT_00d3b530);
          return DAT_00d3b544;
        }
        if (DAT_00d3b5be == 10) {
          FUN_0051f780(DAT_00d3b5b8,DAT_00d3b530);
          return DAT_00d3b544;
        }
        FUN_00410090(DAT_00d3b5b8);
        return DAT_00d3b544;
      }
      if (DAT_00d3b531 == '\x02') {
        if (DAT_00d3b5be == 0x3a) {
          FUN_0042b440(DAT_00d3b5b8,DAT_00d3b530);
          return DAT_00d3b544;
        }
        if (DAT_00d3b5be == 0x1a) {
          FUN_0051f780(DAT_00d3b5b8,DAT_00d3b530);
          return DAT_00d3b544;
        }
        if (DAT_00d3b5be == 10) {
          FUN_0051f780(DAT_00d3b5b8,DAT_00d3b530);
          return DAT_00d3b544;
        }
        FUN_0040ff50(DAT_00d3b5b8);
        return DAT_00d3b544;
      }
      if (DAT_00d3b5be == 0x3a) {
        FUN_00427a20(DAT_00d3b5b8,DAT_00d3b530);
        return DAT_00d3b544;
      }
      if (DAT_00d3b5be == 0x1a) {
        FUN_0051f780(DAT_00d3b5b8,DAT_00d3b530);
        return DAT_00d3b544;
      }
      if (DAT_00d3b5be == 10) {
        FUN_0051f780(DAT_00d3b5b8,DAT_00d3b530);
        return DAT_00d3b544;
      }
      goto LAB_00540387;
    }
    if (DAT_00d3b5b4 == '\r') {
      if (DAT_00d3b531 != '\0') {
        FUN_00411fd0(DAT_00d3b5b8);
        return DAT_00d3b544;
      }
    }
    else if (DAT_00d3b5b4 == '\f') {
      if (DAT_00d3b531 != '\0') {
        FUN_004114a0(DAT_00d3b5b8);
        return DAT_00d3b544;
      }
    }
    else if (DAT_00d3b5b4 == '\n') {
      if (DAT_00d3b531 != '\0') {
        FUN_00410fb0(DAT_00d3b5b8);
        return DAT_00d3b544;
      }
    }
    else if (DAT_00d3b5b4 == '\x02') {
      if (DAT_00d3b531 != '\0') {
        FUN_00410f70(DAT_00d3b5b8);
        return DAT_00d3b544;
      }
    }
    else if (DAT_00d3b5b4 == '\x14') {
      if (DAT_00d3b531 == '\x01') {
        FUN_00410de0(DAT_00d3b5b8);
        return DAT_00d3b544;
      }
      if (DAT_00d3b531 == '\x02') {
        FUN_00410e80(DAT_00d3b5b8);
        return DAT_00d3b544;
      }
      FUN_00410b10(DAT_00d3b5b8);
      return DAT_00d3b544;
    }
    DAT_00d3b5b8 = DAT_00d3b5b8 + (DAT_00d3b5be * DAT_00d3b538 + DAT_00d3b54c) * 2;
    if (DAT_00d3b531 == '\x01') {
      FUN_0040ff50(DAT_00d3b5b8);
      return DAT_00d3b544;
    }
  }
  if (DAT_00d3b531 == '\x02') {
    FUN_0040ff50();
    return DAT_00d3b544;
  }
LAB_00540387: ;
  FUN_0040fed0(DAT_00d3b5b8);
  return DAT_00d3b544;
}
