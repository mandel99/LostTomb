#include "restore_types.h"
#define DAT_005da18c (*(undefined4 *)0x005da18cu)
#define DAT_005da19c (*(undefined4 *)0x005da19cu)
#define DAT_005da1b4 (*(undefined4 *)0x005da1b4u)
#define DAT_005da1b8 (*(undefined4 *)0x005da1b8u)
#define DAT_005da1bc (*(undefined4 *)0x005da1bcu)
#define DAT_005da1c4 (*(undefined4 *)0x005da1c4u)
#define DAT_007027d4 (*(undefined4 *)0x007027d4u)
#define DAT_00785c40 (*(undefined4 *)0x00785c40u)
#define DAT_00785c68 (*(undefined1 *)0x00785c68u)
#define DAT_00785d70 (*(undefined *)0x00785d70u)
#define DAT_00785d74 (*(undefined4 *)0x00785d74u)
#define DAT_00e20768 (*(uint32_t *)0x00e20768u)
#define PTR_DAT_005da1a0 (*(unsigned char * *)0x005da1a0u)
#define PTR_DAT_005da1ac (*(unsigned char * *)0x005da1acu)
#define PTR_DAT_005da1b0 (*(unsigned char * *)0x005da1b0u)
#define s__d__d_005da1a4 (*(char (*)[7])0x005da1a4u)
#define s__s_s_s_005da194 (*(char (*)[7])0x005da194u)
extern undefined1 FUN_004493b0();
extern undefined4 FUN_004493d0();
extern undefined4 FUN_004495c0();
extern undefined1 FUN_004495f0();
extern undefined1 FUN_00449600();
extern undefined1 FUN_00449620();
extern undefined1 FUN_004496b0();
extern void FUN_004cc110();
extern undefined4 FUN_00561a36();

undefined * FUN_00448f60(void)

{
  char cVar1;
  short sVar2;
  int iVar3;
  short sVar4;
  short *in_ECX;
  uint uVar5;
  uint uVar6;
  int *piVar7;
  short *psVar8;
  undefined1 *puVar9;
  undefined2 *puVar10;
  char *pcVar11;
  undefined1 *puVar12;
  char *pcVar13;
  char *pcVar14;
  undefined *puVar15;
  short *local_15c;
  short local_158 [4];
  int local_150;
  undefined1 *local_14c;
  undefined1 *local_148;
  char local_144 [20];
  int local_130 [3];
  char local_124 [52];
  undefined1 local_f0 [40];
  char local_c8 [200];
  
  local_158[0] = -1;
  local_158[1] = -1;
  local_158[2] = -1;
  local_148 = DAT_00e20768;
  FUN_004cc110(0x19,*(undefined1 *)((int)in_ECX + 3));
  local_14c = DAT_00e20768;
  FUN_004cc110(0x14,0);
  FUN_004cc110(0x14,0);
  local_150 = FUN_004495c0();
  if (-1 < local_150) {
    local_158[0] = in_ECX[3];
    local_158[1] = in_ECX[4];
    local_158[2] = in_ECX[5];
    piVar7 = local_130;
    psVar8 = local_158;
    local_15c = (short *)0x3;
    do {
      sVar2 = *psVar8;
      iVar3 = FUN_004493d0();
      if (sVar2 < iVar3) {
        DAT_00e20768 = &DAT_007027d4;
      }
      else {
        FUN_004cc110(local_150,(int)sVar2);
      }
      *piVar7 = (int)DAT_00e20768;
      psVar8 = psVar8 + 1;
      piVar7 = piVar7 + 1;
      local_15c = (short *)((int)local_15c + -1);
    } while (local_15c != (short *)0x0);
  }
  DAT_00e20768 = local_148;
  if ((*(byte *)(in_ECX + 0x14) & 1) == 0) {
    if ((*(uint *)(in_ECX + 0x14) >> 4 & 1) == 0) {
      puVar15 = &DAT_005da1b4;
      puVar9 = local_14c;
      goto LAB_00449104;
    }
    FUN_00561a36(local_124,&DAT_005da1b8);
  }
  else {
    puVar9 = (undefined1 *)0xffffffff;
    if (0 < DAT_00785d70) {
      puVar10 = &DAT_00785d74;
      puVar12 = (undefined1 *)0x0;
      do {
        puVar9 = puVar12;
        cVar1 = FUN_004495f0();
        if (((((cVar1 != '\0') && (puVar10[0x12] == *in_ECX)) ||
             ((cVar1 = FUN_004496b0(), cVar1 != '\0' && (puVar10[0x13] == *in_ECX)))) ||
            ((cVar1 = FUN_004496b0(), cVar1 != '\0' && (puVar10[0x29] == *in_ECX)))) ||
           ((cVar1 = FUN_00449600(), cVar1 != '\0' && (puVar10[0x2a] == *in_ECX)))) {
          if (-1 < (int)puVar9) {
            puVar15 = &DAT_005da1bc;
            goto LAB_00449104;
          }
          break;
        }
        puVar12 = puVar9 + 1;
        puVar10 = puVar10 + 0x3e;
        puVar9 = (undefined1 *)0xffffffff;
      } while ((int)puVar12 < DAT_00785d70);
    }
    puVar15 = &DAT_005da1c4;
LAB_00449104: ;
    FUN_00561a36(local_124,puVar15,puVar9);
  }
  sVar2 = in_ECX[0xb];
  if (sVar2 < 0) {
    sVar2 = in_ECX[0xc];
    sVar4 = in_ECX[0xd];
  }
  else {
    sVar4 = sVar2;
    if (-1 < in_ECX[0xc]) {
      sVar2 = -1;
      sVar4 = -1;
    }
  }
  FUN_00561a36(local_144,&PTR_DAT_005da1b0,(int)sVar2);
  uVar5 = 0xffffffff;
  pcVar11 = local_144;
  do {
    pcVar14 = pcVar11;
    if (uVar5 == 0) break;
    uVar5 = uVar5 - 1;
    pcVar14 = pcVar11 + 1;
    cVar1 = *pcVar11;
    pcVar11 = pcVar14;
  } while (cVar1 != '\0');
  uVar5 = ~uVar5;
  iVar3 = -1;
  pcVar11 = local_124;
  do {
    pcVar13 = pcVar11;
    if (iVar3 == 0) break;
    iVar3 = iVar3 + -1;
    pcVar13 = pcVar11 + 1;
    cVar1 = *pcVar11;
    pcVar11 = pcVar13;
  } while (cVar1 != '\0');
  pcVar11 = pcVar14 + -uVar5;
  pcVar14 = pcVar13 + -1;
  for (uVar6 = uVar5 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
    *(undefined4 *)pcVar14 = *(undefined4 *)pcVar11;
    pcVar11 = pcVar11 + 4;
    pcVar14 = pcVar14 + 4;
  }
  for (uVar5 = uVar5 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
    *pcVar14 = *pcVar11;
    pcVar11 = pcVar11 + 1;
    pcVar14 = pcVar14 + 1;
  }
  if (sVar2 < sVar4) {
    FUN_00561a36(local_144,&PTR_DAT_005da1ac,(int)sVar4);
    uVar5 = 0xffffffff;
    pcVar11 = local_144;
    do {
      pcVar14 = pcVar11;
      if (uVar5 == 0) break;
      uVar5 = uVar5 - 1;
      pcVar14 = pcVar11 + 1;
      cVar1 = *pcVar11;
      pcVar11 = pcVar14;
    } while (cVar1 != '\0');
    uVar5 = ~uVar5;
    iVar3 = -1;
    pcVar11 = local_124;
    do {
      pcVar13 = pcVar11;
      if (iVar3 == 0) break;
      iVar3 = iVar3 + -1;
      pcVar13 = pcVar11 + 1;
      cVar1 = *pcVar11;
      pcVar11 = pcVar13;
    } while (cVar1 != '\0');
    pcVar11 = pcVar14 + -uVar5;
    pcVar14 = pcVar13 + -1;
    for (uVar6 = uVar5 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
      *(undefined4 *)pcVar14 = *(undefined4 *)pcVar11;
      pcVar11 = pcVar11 + 4;
      pcVar14 = pcVar14 + 4;
    }
    for (uVar5 = uVar5 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
      *pcVar14 = *pcVar11;
      pcVar11 = pcVar11 + 1;
      pcVar14 = pcVar14 + 1;
    }
  }
  cVar1 = FUN_00449620();
  if (cVar1 == '\0') {
    local_f0[0] = 0;
    goto LAB_00449237;
  }
  sVar2 = in_ECX[7];
  if (sVar2 < 0) {
    sVar2 = in_ECX[8];
    sVar4 = in_ECX[9];
LAB_004491e5: ;
    if (sVar2 < sVar4) {
      FUN_00561a36(local_f0,s__d__d_005da1a4,(int)sVar2,(int)sVar4);
      goto LAB_00449237;
    }
  }
  else {
    sVar4 = sVar2;
    if (in_ECX[8] < 0) goto LAB_004491e5;
    sVar2 = -1;
  }
  FUN_00561a36(local_f0,&PTR_DAT_005da1a0,(int)sVar2);
LAB_00449237: ;
  local_15c = local_158;
  local_c8[0] = ' ';
  local_c8[1] = '\0';
  piVar7 = local_130;
  local_150 = 3;
  do {
    if (0 < *local_15c) {
      if (local_c8[1] != '\0') {
        uVar5 = 0xffffffff;
        pcVar11 = &DAT_005da19c;
        do {
          pcVar14 = pcVar11;
          if (uVar5 == 0) break;
          uVar5 = uVar5 - 1;
          pcVar14 = pcVar11 + 1;
          cVar1 = *pcVar11;
          pcVar11 = pcVar14;
        } while (cVar1 != '\0');
        uVar5 = ~uVar5;
        iVar3 = -1;
        pcVar11 = local_c8;
        do {
          pcVar13 = pcVar11;
          if (iVar3 == 0) break;
          iVar3 = iVar3 + -1;
          pcVar13 = pcVar11 + 1;
          cVar1 = *pcVar11;
          pcVar11 = pcVar13;
        } while (cVar1 != '\0');
        pcVar11 = pcVar14 + -uVar5;
        pcVar14 = pcVar13 + -1;
        for (uVar6 = uVar5 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
          *(undefined4 *)pcVar14 = *(undefined4 *)pcVar11;
          pcVar11 = pcVar11 + 4;
          pcVar14 = pcVar14 + 4;
        }
        for (uVar5 = uVar5 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
          *pcVar14 = *pcVar11;
          pcVar11 = pcVar11 + 1;
          pcVar14 = pcVar14 + 1;
        }
      }
      uVar5 = 0xffffffff;
      pcVar11 = (char *)*piVar7;
      do {
        pcVar14 = pcVar11;
        if (uVar5 == 0) break;
        uVar5 = uVar5 - 1;
        pcVar14 = pcVar11 + 1;
        cVar1 = *pcVar11;
        pcVar11 = pcVar14;
      } while (cVar1 != '\0');
      uVar5 = ~uVar5;
      iVar3 = -1;
      pcVar11 = local_c8;
      do {
        pcVar13 = pcVar11;
        if (iVar3 == 0) break;
        iVar3 = iVar3 + -1;
        pcVar13 = pcVar11 + 1;
        cVar1 = *pcVar11;
        pcVar11 = pcVar13;
      } while (cVar1 != '\0');
      pcVar11 = pcVar14 + -uVar5;
      pcVar14 = pcVar13 + -1;
      for (uVar6 = uVar5 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
        *(undefined4 *)pcVar14 = *(undefined4 *)pcVar11;
        pcVar11 = pcVar11 + 4;
        pcVar14 = pcVar14 + 4;
      }
      for (uVar5 = uVar5 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
        *pcVar14 = *pcVar11;
        pcVar11 = pcVar11 + 1;
        pcVar14 = pcVar14 + 1;
      }
    }
    local_15c = local_15c + 1;
    piVar7 = piVar7 + 1;
    local_150 = local_150 + -1;
    if (local_150 == 0) {
      cVar1 = FUN_004493b0();
      if (cVar1 == '\0') {
        FUN_00561a36(&DAT_00785c40,&DAT_005da18c,local_124,local_f0);
        DAT_00785c68 = 0;
        return &DAT_00785c40;
      }
      FUN_00561a36(&DAT_00785c40,s__s_s_s_005da194,local_124,local_f0,local_c8);
      DAT_00785c68 = 0;
      return &DAT_00785c40;
    }
  } while( true );
}
