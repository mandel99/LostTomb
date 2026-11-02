#include "restore_types.h"
#define DAT_005e05ac (*(undefined *)0x005e05acu)
#define DAT_005e06e0 (*(undefined4 *)0x005e06e0u)
#define DAT_005e070c (*(undefined4 *)0x005e070cu)
#define DAT_00e38aec (*(char *)0x00e38aecu)
#define DAT_00e38e62 (*(undefined1 *)0x00e38e62u)
extern void FUN_004cbec0();
extern bool FUN_004ce100();
extern void FUN_004ce2d0();
extern void FUN_004ce330();
extern int FUN_004ce9e0(void);
extern undefined4 FUN_005647eb();
extern int FUN_005660ed();
extern DWORD FUN_005662e3();
extern uint FUN_00568eab();

int FUN_004ce480(char *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  char cVar1;
  byte bVar2;
  int iVar3;
  byte *pbVar4;
  uint uVar5;
  uint uVar6;
  char *pcVar7;
  int iVar8;
  byte *pbVar9;
  char *pcVar10;
  char *pcVar11;
  bool bVar12;
  int local_48;
  char local_44 [4];
  char local_40 [64];
  
  local_48 = 0;
  iVar8 = 1;
  iVar3 = FUN_00568eab(param_1,0x8000);
  if (iVar3 == -1) {
    RESTORE_SLICE1(DAT_005e05ac,0) = DAT_00e38e62;
    uVar5 = 0xffffffff;
    RESTORE_SLICE1(DAT_005e05ac,3) = 0;
    pcVar7 = param_1;
    do {
      pcVar10 = pcVar7;
      if (uVar5 == 0) break;
      uVar5 = uVar5 - 1;
      pcVar10 = pcVar7 + 1;
      cVar1 = *pcVar7;
      pcVar7 = pcVar10;
    } while (cVar1 != '\0');
    uVar5 = ~uVar5;
    iVar3 = -1;
    pcVar7 = (char *)&DAT_005e05ac;
    do {
      pcVar11 = pcVar7;
      if (iVar3 == 0) break;
      iVar3 = iVar3 + -1;
      pcVar11 = pcVar7 + 1;
      cVar1 = *pcVar7;
      pcVar7 = pcVar11;
    } while (cVar1 != '\0');
    pcVar7 = pcVar10 + -uVar5;
    pcVar10 = pcVar11 + -1;
    for (uVar6 = uVar5 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
      *(undefined4 *)pcVar10 = *(undefined4 *)pcVar7;
      pcVar7 = pcVar7 + 4;
      pcVar10 = pcVar10 + 4;
    }
    for (uVar5 = uVar5 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
      *pcVar10 = *pcVar7;
      pcVar7 = pcVar7 + 1;
      pcVar10 = pcVar10 + 1;
    }
    iVar3 = FUN_00568eab(&DAT_005e05ac,0x8000);
    while (iVar3 == -1) {
      if (iVar8 != 1) goto LAB_004ce573;
      iVar8 = FUN_004ce9e0();
      iVar3 = FUN_00568eab(&DAT_005e05ac,0x8000);
    }
  }
  iVar8 = FUN_005662e3(iVar3,param_4,0);
  if (iVar8 != -1) {
    local_48 = FUN_005660ed(iVar3,param_2,param_3);
  }
  FUN_005647eb(iVar3);
  if (local_48 < 1) {
LAB_004ce573: ;
    pcVar7 = local_44;
    for (iVar8 = 0x10; iVar8 != 0; iVar8 = iVar8 + -1) {
      pcVar7[0] = '\0';
      pcVar7[1] = '\0';
      pcVar7[2] = '\0';
      pcVar7[3] = '\0';
      pcVar7 = pcVar7 + 4;
    }
    *pcVar7 = '\0';
    FUN_004ce330(param_1);
    FUN_004cbec0(&DAT_00e38aec);
    pbVar9 = &DAT_00e38aec;
    pbVar4 = &DAT_005e06e0;
    do {
      bVar2 = *pbVar4;
      bVar12 = bVar2 < *pbVar9;
      if (bVar2 != *pbVar9) {
LAB_004ce5c2: ;
        iVar8 = (1 - (uint)bVar12) - (uint)(bVar12 != 0);
        goto LAB_004ce5c7;
      }
      if (bVar2 == 0) break;
      bVar2 = pbVar4[1];
      bVar12 = bVar2 < pbVar9[1];
      if (bVar2 != pbVar9[1]) goto LAB_004ce5c2;
      pbVar4 = pbVar4 + 2;
      pbVar9 = pbVar9 + 2;
    } while (bVar2 != 0);
    iVar8 = 0;
LAB_004ce5c7: ;
    pcVar7 = local_44;
    if (iVar8 == 0) {
      uVar5 = 0xffffffff;
      pcVar7 = &DAT_005e070c;
      do {
        pcVar10 = pcVar7;
        if (uVar5 == 0) break;
        uVar5 = uVar5 - 1;
        pcVar10 = pcVar7 + 1;
        cVar1 = *pcVar7;
        pcVar7 = pcVar10;
      } while (cVar1 != '\0');
      uVar5 = ~uVar5;
      pcVar7 = local_44 + 4;
      pcVar10 = pcVar10 + -uVar5;
      pcVar11 = local_44;
      for (uVar6 = uVar5 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
        *(undefined4 *)pcVar11 = *(undefined4 *)pcVar10;
        pcVar10 = pcVar10 + 4;
        pcVar11 = pcVar11 + 4;
      }
      for (uVar5 = uVar5 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
        *pcVar11 = *pcVar10;
        pcVar10 = pcVar10 + 1;
        pcVar11 = pcVar11 + 1;
      }
    }
    uVar5 = 0xffffffff;
    do {
      pcVar10 = param_1;
      if (uVar5 == 0) break;
      uVar5 = uVar5 - 1;
      pcVar10 = param_1 + 1;
      cVar1 = *param_1;
      param_1 = pcVar10;
    } while (cVar1 != '\0');
    uVar5 = ~uVar5;
    pcVar10 = pcVar10 + -uVar5;
    for (uVar6 = uVar5 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
      *(undefined4 *)pcVar7 = *(undefined4 *)pcVar10;
      pcVar10 = pcVar10 + 4;
      pcVar7 = pcVar7 + 4;
    }
    for (uVar5 = uVar5 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
      *pcVar7 = *pcVar10;
      pcVar10 = pcVar10 + 1;
      pcVar7 = pcVar7 + 1;
    }
    iVar8 = FUN_00568eab(local_44,0x8000);
    if (iVar8 != -1) {
      iVar3 = FUN_005662e3(iVar8,param_4,0);
      if (iVar3 == -1) {
        FUN_005647eb(iVar8);
      }
      else {
        local_48 = FUN_005660ed(iVar8,param_2,param_3);
        FUN_005647eb(iVar8);
      }
    }
    if (local_48 < 1) {
      FUN_004ce100(local_44);
      iVar8 = FUN_00568eab(local_44,0x8000);
      if (iVar8 != -1) {
        iVar3 = FUN_005662e3(iVar8,param_4,0);
        if (iVar3 != -1) {
          local_48 = FUN_005660ed(iVar8,param_2,param_3);
        }
        FUN_005647eb(iVar8);
      }
      FUN_004ce2d0();
    }
  }
  return local_48;
}
