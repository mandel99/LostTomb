#include "restore_types.h"
#define DAT_0056f7d8 (*(undefined4 *)0x0056f7d8u)
#define DAT_0056f7ec (*(undefined4 *)0x0056f7ecu)
#define DAT_0056f7f8 (*(undefined4 *)0x0056f7f8u)
#define DAT_0056f80c (*(undefined4 *)0x0056f80cu)
#define DAT_0056f820 (*(undefined4 *)0x0056f820u)
#define DAT_0056f82c (*(undefined4 *)0x0056f82cu)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_005e4efc (*(undefined4 *)0x005e4efcu)
#define DAT_005e657c (*(undefined4 *)0x005e657cu)
#define DAT_005e68d8 (*(undefined4 *)0x005e68d8u)
#define DAT_005f5c40 (*(undefined4 *)0x005f5c40u)
#define DAT_005f5c54 (*(undefined4 *)0x005f5c54u)
#define DAT_005f5c68 (*(undefined4 *)0x005f5c68u)
#define DAT_005f5c7c (*(undefined4 *)0x005f5c7cu)
#define DAT_005f5c88 (*(undefined4 *)0x005f5c88u)
#define DAT_005f5c9c (*(undefined4 *)0x005f5c9cu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00799aa0 (*(undefined4 *)0x00799aa0u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd660 (*(undefined4 *)0x00bcd660u)
#define DAT_00eaac44 (*(undefined *)0x00eaac44u)
#define DAT_00eaaf58 (*(undefined *)0x00eaaf58u)
extern uint FUN_004eea10();
extern void FUN_004ef450();
extern int FUN_004efd90();
extern uint FUN_004f26a0();

void FUN_004f1310(int param_1,int *param_2)

{
  short sVar1;
  short sVar2;
  int *piVar3;
  char cVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  uint uVar8;
  int iVar9;
  uint uVar10;
  int iVar11;
  int iVar12;
  undefined4 *puVar13;
  int *piVar14;
  byte *pbVar15;
  int local_48;
  int local_40 [16];
  
  piVar3 = param_2;
  piVar14 = param_2;
  for (iVar7 = 0x1c; iVar7 != 0; iVar7 = iVar7 + -1) {
    *piVar14 = 0;
    piVar14 = piVar14 + 1;
  }
  if ((&DAT_007999e8)[param_1 * 0x84] != 0xb7) {
    return;
  }
  param_1 = param_1 - (short)(&DAT_007999ea)[param_1 * 0x84];
  uVar8 = (uint)*(byte *)(&DAT_00799a2c + param_1 * 0x84);
  if (DAT_005e4ef4 < 1) {
LAB_004f138b: ;
    iVar7 = 0;
  }
  else if (DAT_005e4ef4 < 5) {
    iVar7 = *(int *)(&DAT_005f5c88 + uVar8 * 4);
  }
  else {
    if (DAT_005e4ef4 != 5) goto LAB_004f138b;
    iVar7 = *(int *)(&DAT_005f5c9c + uVar8 * 4);
  }
  param_2[0x11] = iVar7;
  uVar5 = (uint)*(byte *)(&DAT_00799a2c + param_1 * 0x84);
  switch(DAT_005e4ef4 + -1) {
  case 0: ;
    iVar12 = *(int *)(&DAT_005f5c68 + uVar5 * 4);
    break;
  case 1: ;
  case 3: ;
    iVar12 = *(int *)(&DAT_005f5c40 + uVar5 * 4);
    break;
  case 2: ;
    iVar12 = *(int *)(&DAT_005f5c54 + uVar5 * 4);
    break;
  case 4: ;
    iVar12 = *(int *)(&DAT_005f5c7c + uVar5 * 4);
    break;
default: ;
    iVar12 = 0;
  }
  switch(DAT_005e4ef4 + -1) {
  case 0: ;
    param_2 = *(int **)(&DAT_0056f82c + uVar8 * 4);
    break;
  case 1: ;
  case 3: ;
    if (DAT_005e4ef4 == 5) {
      param_2 = (int *)(*(int *)(&DAT_0056f7ec + uVar8 * 4) * 6);
    }
    else {
      param_2 = (int *)(*(int *)(&DAT_0056f7d8 + uVar8 * 4) * 6);
    }
    break;
default: ;
    param_2 = (int *)0x0;
  }
  if (1 < *(byte *)((int)&DAT_00799a2c + param_1 * 0x108 + 1)) {
    iVar7 = 0;
    *piVar3 = 1;
    piVar3[0xf] = 0;
    piVar3[0xe] = 999;
    piVar3[0x10] = 0;
    local_40[0] = 0;
    local_40[1] = 1;
    local_40[2] = 1;
    local_40[3] = 2;
    local_40[4] = 1;
    local_40[5] = 2;
    local_40[6] = 2;
    local_40[7] = 3;
    local_40[8] = 1;
    local_40[9] = 2;
    local_40[10] = 2;
    local_40[0xb] = 3;
    local_40[0xc] = 2;
    local_40[0xd] = 3;
    local_40[0xe] = 3;
    local_40[0xf] = 4;
    pbVar15 = (byte *)((int)&DAT_00799a30 + param_1 * 0x108 + 3);
    while( true ) {
      if (DAT_005e4ef4 == 5) {
        iVar12 = *(int *)(&DAT_0056f7ec + uVar8 * 4);
      }
      else {
        iVar12 = *(int *)(&DAT_0056f7d8 + uVar8 * 4);
      }
      if (iVar12 <= iVar7) break;
      uVar5 = (uint)pbVar15[-6];
      if ((int)(uint)pbVar15[-6] <= piVar3[0xf]) {
        uVar5 = piVar3[0xf];
      }
      piVar3[0xf] = uVar5;
      uVar5 = (uint)pbVar15[-6];
      if (piVar3[0xe] <= (int)(uint)pbVar15[-6]) {
        uVar5 = piVar3[0xe];
      }
      piVar3[0xe] = uVar5;
      if (((*(short *)(pbVar15 + 9) != 0) &&
          ((&DAT_00bcd602)[*(short *)(pbVar15 + 9) * 0x184] == 'U')) &&
         (cVar4 = FUN_004eea10(param_1 + iVar7), cVar4 != '\0')) {
        piVar3[0x19] = piVar3[0x19] + 1;
      }
      switch(pbVar15[-6]) {
      case 4: ;
        iVar7 = iVar7 + 1;
        piVar3[0x10] = piVar3[0x10] + local_40[*pbVar15];
        pbVar15 = pbVar15 + 0x108;
        break;
      case 5: ;
        iVar7 = iVar7 + 1;
        piVar3[0x10] = piVar3[0x10] + 5;
        pbVar15 = pbVar15 + 0x108;
        break;
      case 6: ;
        iVar7 = iVar7 + 1;
        piVar3[0x10] = piVar3[0x10] + local_40[*pbVar15] + 5;
        pbVar15 = pbVar15 + 0x108;
        break;
      case 7: ;
        iVar7 = iVar7 + 1;
        piVar3[0x10] = piVar3[0x10] + local_40[*pbVar15] + 9;
        pbVar15 = pbVar15 + 0x108;
        break;
      case 8: ;
        iVar7 = iVar7 + 1;
        piVar3[0x10] = piVar3[0x10] + local_40[*pbVar15] + 0xd;
        pbVar15 = pbVar15 + 0x108;
        break;
      case 9: ;
        iVar7 = iVar7 + 1;
        piVar3[0x10] = piVar3[0x10] + local_40[*pbVar15] + 0x11;
        pbVar15 = pbVar15 + 0x108;
        break;
      case 10: ;
        piVar3[0x10] = piVar3[0x10] + local_40[*pbVar15] + 0x15;
default: ;
        iVar7 = iVar7 + 1;
        pbVar15 = pbVar15 + 0x108;
      }
    }
    if (piVar3[0xf] == 0xb) {
      piVar3[0x10] = piVar3[0x10] + 1;
    }
    iVar7 = (piVar3[0x10] * 100) / piVar3[0x11];
    piVar3[0xc] = iVar7;
    iVar12 = 0;
    piVar3[0xd] = (iVar7 * 5) / 100;
    while( true ) {
      if (DAT_005e4ef4 == 5) {
        iVar7 = *(int *)(&DAT_0056f7ec + uVar8 * 4);
      }
      else {
        iVar7 = *(int *)(&DAT_0056f7d8 + uVar8 * 4);
      }
      if (iVar7 <= iVar12) break;
      if (DAT_005e4ef4 == 5) {
        iVar7 = *(int *)(&DAT_005e657c + (iVar12 + uVar8 * 0x24) * 8);
        if (iVar12 == 0) {
          iVar12 = 1;
          piVar3[10] = piVar3[10] + 1;
        }
        else {
LAB_004f16e8: ;
          iVar7 = piVar3[10] + iVar7 * 6;
          piVar3[10] = iVar7;
          if (param_2 == (int *)0x0) {
            iVar12 = iVar12 + 1;
            piVar3[10] = iVar7 + 6;
          }
          else {
            iVar7 = piVar3[0xb];
LAB_004f1702: ;
            iVar12 = iVar12 + 1;
            piVar3[0xb] = iVar7 + 6;
          }
        }
      }
      else {
        iVar7 = *(int *)(&DAT_005e4efc + (uVar8 * 0x90 + iVar12) * 8);
        if (DAT_005e4ef4 != 1) goto LAB_004f16e8;
        iVar9 = 0;
        if (0 < iVar7) {
          do {
            if ((&DAT_005e68d8)[uVar8 * 6 + iVar9] == '\0') {
              piVar3[10] = piVar3[10] + 6;
            }
            else {
              piVar3[10] = piVar3[10] + 3;
            }
            iVar9 = iVar9 + 1;
          } while (iVar9 < iVar7);
        }
        iVar7 = piVar3[0xb];
        if ((&DAT_005e68d8)[uVar8 * 6 + (uint)*(byte *)(&DAT_00799a30 + (iVar12 + param_1) * 0x42)]
            == '\0') goto LAB_004f1702;
        iVar12 = iVar12 + 1;
        piVar3[0xb] = iVar7 + 3;
      }
    }
    goto LAB_004f1bc2;
  }
  piVar3[0x10] = iVar7;
  FUN_004ef450(param_1);
  iVar7 = DAT_00eaac44;
  if (DAT_00eaaf58 < 1) {
    iVar7 = param_1;
  }
  iVar9 = (uint)*(byte *)(&DAT_00799a2e + iVar7 * 0x84) +
          (uint)*(byte *)((int)&DAT_00799a2e + iVar7 * 0x108 + 1) * 6;
  piVar3[1] = iVar9;
  piVar3[2] = iVar9 + 1;
  if (DAT_005e4ef4 == 1) {
    iVar9 = *(int *)(&DAT_0056f80c + uVar8 * 4);
LAB_004f1796: ;
    piVar3[3] = iVar9;
  }
  else {
    if (DAT_005e4ef4 != 5) {
      iVar9 = *(int *)(&DAT_0056f7f8 + uVar8 * 4);
      goto LAB_004f1796;
    }
    piVar3[3] = *(int *)(&DAT_0056f820 + uVar8 * 4);
  }
  if (DAT_00eaaf58 < 1) {
    *piVar3 = 4;
    piVar3[0xc] = 100;
    piVar3[0xd] = 100;
  }
  else {
    sVar1 = *(short *)(&DAT_00799aa0 + param_1 * 0x42);
    if (*(char *)((int)&DAT_00799a2c + iVar7 * 0x108 + 1) == '\0') {
      *piVar3 = 2;
      sVar2 = *(short *)(&DAT_00799aa0 + param_1 * 0x42);
      iVar7 = iVar12;
      iVar9 = iVar12 - (int)param_2;
    }
    else {
      *piVar3 = 3;
      sVar2 = *(short *)(&DAT_00799aa0 + param_1 * 0x42);
      iVar7 = (int)param_2;
      iVar9 = (int)param_2;
    }
    piVar3[0xc] = ((iVar7 - sVar2) * 100) / iVar9;
    piVar3[0xd] = ((iVar12 - sVar1) * 0x5f) / iVar12 + 5;
  }
  local_48 = 0;
  while( true ) {
    if (DAT_005e4ef4 == 5) {
      iVar7 = *(int *)(&DAT_0056f7ec + uVar8 * 4);
    }
    else {
      iVar7 = *(int *)(&DAT_0056f7d8 + uVar8 * 4);
    }
    if (iVar7 <= local_48) break;
    iVar7 = param_1 + local_48;
    if (DAT_005e4ef4 == 5) {
      uVar5 = *(uint *)(&DAT_005e657c + (local_48 + uVar8 * 0x24) * 8);
    }
    else {
      uVar5 = *(uint *)(&DAT_005e4efc + (uVar8 * 0x90 + local_48) * 8);
    }
    iVar12 = iVar7 * 0x108;
    if (*(char *)((int)&DAT_00799a2c + iVar12 + 1) == '\0') {
      uVar6 = (uint)*(byte *)((int)&DAT_00799a2e + iVar12 + 1);
      uVar10 = uVar6;
      if ((int)uVar5 <= (int)uVar6) {
        uVar10 = uVar5;
      }
      if ((int)uVar10 < 0) {
        uVar6 = 0;
      }
      else if ((int)uVar5 <= (int)uVar6) {
        uVar6 = uVar5;
      }
      if (DAT_005e4ef4 == 1) {
        iVar9 = 0;
        if (0 < (int)uVar6) {
          do {
            if ((&DAT_005e68d8)[uVar8 * 6 + iVar9] == '\0') {
              piVar3[4] = piVar3[4] + 6;
            }
            else {
              piVar3[4] = piVar3[4] + 3;
            }
            iVar9 = iVar9 + 1;
          } while (iVar9 < (int)uVar6);
        }
      }
      else if ((DAT_005e4ef4 != 5) || (local_48 != 0)) {
        piVar3[4] = piVar3[4] + uVar6 * 6;
      }
      if ((*(byte *)((int)&DAT_00799a2e + iVar12 + 1) < *(byte *)(&DAT_00799a30 + iVar7 * 0x42)) ||
         (param_2 == (int *)0x0)) {
        piVar3[4] = piVar3[4] + (uint)*(byte *)(&DAT_00799a2e + iVar7 * 0x84);
      }
      else {
        piVar3[5] = piVar3[5] + (uint)*(byte *)(&DAT_00799a2e + iVar7 * 0x84);
      }
LAB_004f19d1: ;
      if (DAT_005e4ef4 == 5) {
        if (local_48 == 0) goto LAB_004f19df;
LAB_004f1a34: ;
        iVar12 = piVar3[10] + uVar5 * 6;
        piVar3[10] = iVar12;
        if (param_2 == (int *)0x0) {
          iVar12 = iVar12 + 6;
          goto LAB_004f1a59;
        }
        iVar12 = piVar3[0xb];
LAB_004f1a4e: ;
        piVar3[0xb] = iVar12 + 6;
      }
      else {
        if (DAT_005e4ef4 != 1) goto LAB_004f1a34;
        iVar12 = 0;
        if (0 < (int)uVar5) {
          do {
            if ((&DAT_005e68d8)[uVar8 * 6 + iVar12] == '\0') {
              iVar9 = piVar3[10] + 6;
            }
            else {
              iVar9 = piVar3[10] + 3;
            }
            iVar12 = iVar12 + 1;
            piVar3[10] = iVar9;
          } while (iVar12 < (int)uVar5);
        }
        iVar12 = piVar3[0xb];
        if ((&DAT_005e68d8)[uVar8 * 6 + (uint)*(byte *)(&DAT_00799a30 + iVar7 * 0x42)] == '\0')
        goto LAB_004f1a4e;
        piVar3[0xb] = iVar12 + 3;
      }
    }
    else {
      if (DAT_005e4ef4 == 1) {
        iVar9 = 0;
        if (0 < (int)uVar5) {
          do {
            if ((&DAT_005e68d8)[uVar8 * 6 + iVar9] == '\0') {
              iVar11 = piVar3[4] + 6;
            }
            else {
              iVar11 = piVar3[4] + 3;
            }
            iVar9 = iVar9 + 1;
            piVar3[4] = iVar11;
          } while (iVar9 < (int)uVar5);
        }
        if ((&DAT_005e68d8)[uVar8 * 6 + (uint)*(byte *)((int)&DAT_00799a2e + iVar12 + 1)] == '\0') {
          piVar3[5] = piVar3[5] + 6;
        }
        else {
          piVar3[5] = piVar3[5] + 3;
        }
        goto LAB_004f19d1;
      }
      if ((DAT_005e4ef4 == 5) && (local_48 == 0)) {
        iVar12 = piVar3[4] + 1;
      }
      else {
        iVar12 = piVar3[4] + uVar5 * 6;
      }
      piVar3[4] = iVar12;
      if (param_2 != (int *)0x0) {
        piVar3[5] = piVar3[5] + 6;
        goto LAB_004f19d1;
      }
      if ((DAT_005e4ef4 != 5) || (local_48 != 0)) {
        piVar3[4] = piVar3[4] + 6;
        goto LAB_004f19d1;
      }
LAB_004f19df: ;
      iVar12 = piVar3[10] + 1;
LAB_004f1a59: ;
      piVar3[10] = iVar12;
    }
    iVar12 = (int)(short)(&DAT_00799a3c)[iVar7 * 0x84];
    if (iVar12 != 0) {
      iVar9 = iVar12 * 0x184;
      switch((&DAT_00bcd602)[iVar9]) {
      case 0x4f: ;
        cVar4 = FUN_004eea10(iVar7);
        if (cVar4 != '\0') {
          piVar3[0x16] = piVar3[0x16] + 1;
        }
        break;
      case 0x50: ;
        cVar4 = FUN_004eea10(iVar7);
        if (cVar4 != '\0') {
          piVar3[0x13] = piVar3[0x13] + 1;
          piVar3[0x18] = piVar3[0x18] + 1;
          if ((&DAT_00bcd630)[iVar9] == '\v') {
            sVar1 = *(short *)(&DAT_00bcd660 + iVar9);
joined_r0x004f1acc: ;
            if ((sVar1 == 0) || (cVar4 = FUN_004f26a0((int)sVar1,iVar12), cVar4 == '\0')) {
              piVar3[0x17] = piVar3[0x17] + 1;
              cVar4 = FUN_004efd90(iVar7);
              if ((cVar4 == '\0') && (param_2 != (int *)0x0)) {
                piVar3[0x15] = piVar3[0x15] + 1;
              }
              else {
                piVar3[0x14] = piVar3[0x14] + 1;
              }
            }
          }
        }
        break;
      case 0x51: ;
        cVar4 = FUN_004eea10(iVar7);
        if (cVar4 != '\0') {
          piVar3[0x12] = piVar3[0x12] + 1;
          piVar3[0x18] = piVar3[0x18] + 1;
          if ((&DAT_00bcd630)[iVar9] == '\n') {
            sVar1 = *(short *)(&DAT_00bcd660 + iVar9);
            goto joined_r0x004f1acc;
          }
        }
        break;
      case 0x55: ;
        cVar4 = FUN_004eea10(iVar7);
        if (cVar4 != '\0') {
          piVar3[0x19] = piVar3[0x19] + 1;
        }
      }
    }
    local_48 = local_48 + 1;
  }
  if ((*piVar3 == 2) && (iVar7 = 0, 0 < DAT_00eaaf58)) {
    puVar13 = &DAT_00eaac44;
    do {
      cVar4 = FUN_004efd90(*puVar13);
      if ((cVar4 == '\0') && (param_2 != (int *)0x0)) {
        piVar3[7] = piVar3[7] + 1;
      }
      else {
        piVar3[6] = piVar3[6] + 1;
      }
      iVar7 = iVar7 + 1;
      puVar13 = puVar13 + 1;
    } while (iVar7 < DAT_00eaaf58);
  }
LAB_004f1bc2: ;
  piVar3[8] = (piVar3[10] - piVar3[4]) / 2;
  piVar3[9] = (piVar3[0xb] - piVar3[5]) / 2;
  piVar3[10] = piVar3[10] / 2;
  piVar3[0xb] = piVar3[0xb] / 2;
  piVar3[4] = piVar3[4] / 2;
  piVar3[5] = piVar3[5] / 2;
  piVar3[6] = piVar3[6] / 2;
  piVar3[7] = piVar3[7] / 2;
  piVar3[0x1a] = piVar3[0x14] * 200;
  piVar3[0x1b] = piVar3[0x15] * 200;
  return;
}
