#include "restore_types.h"
#define DAT_005e03f0 (*(undefined4 *)0x005e03f0u)
#define DAT_005e03f8 (*(undefined4 *)0x005e03f8u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_00799ad8 (*(undefined4 *)0x00799ad8u)
#define DAT_00d3a4c0 (*(undefined4 *)0x00d3a4c0u)
#define DAT_00e20768 (*(uint32_t *)0x00e20768u)
extern int FUN_00475b50();
extern void FUN_004cc110();
extern undefined4 FUN_00561a36();

undefined4 FUN_004ca770(undefined4 param_1)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  int iVar8;
  char *pcVar9;
  char *pcVar10;
  char *pcVar11;
  char local_8 [8];
  
  iVar2 = FUN_00475b50(param_1);
  iVar2 = iVar2 * 0x108;
  if (*(int *)(&DAT_00799ad8 + iVar2) == 0) {
    return 0;
  }
  if (*(char *)((int)&DAT_007999dc + iVar2 + 1) == '\0') {
    FUN_004cc110(0x42,0x8e);
    uVar3 = 0xffffffff;
    pcVar9 = DAT_00e20768;
    do {
      pcVar11 = pcVar9;
      if (uVar3 == 0) break;
      uVar3 = uVar3 - 1;
      pcVar11 = pcVar9 + 1;
      cVar1 = *pcVar9;
      pcVar9 = pcVar11;
    } while (cVar1 != '\0');
    uVar3 = ~uVar3;
    pcVar9 = pcVar11 + -uVar3;
    pcVar11 = (char *)&DAT_00d3a4c0;
    for (uVar4 = uVar3 >> 2; uVar4 != 0; uVar4 = uVar4 - 1) {
      *(undefined4 *)pcVar11 = *(undefined4 *)pcVar9;
      pcVar9 = pcVar9 + 4;
      pcVar11 = pcVar11 + 4;
    }
  }
  else {
    FUN_004cc110(0x42,0x8d);
    uVar3 = 0xffffffff;
    pcVar9 = DAT_00e20768;
    do {
      pcVar11 = pcVar9;
      if (uVar3 == 0) break;
      uVar3 = uVar3 - 1;
      pcVar11 = pcVar9 + 1;
      cVar1 = *pcVar9;
      pcVar9 = pcVar11;
    } while (cVar1 != '\0');
    uVar3 = ~uVar3;
    pcVar9 = pcVar11 + -uVar3;
    pcVar11 = (char *)&DAT_00d3a4c0;
    for (uVar4 = uVar3 >> 2; uVar4 != 0; uVar4 = uVar4 - 1) {
      *(undefined4 *)pcVar11 = *(undefined4 *)pcVar9;
      pcVar9 = pcVar9 + 4;
      pcVar11 = pcVar11 + 4;
    }
  }
  for (uVar3 = uVar3 & 3; uVar3 != 0; uVar3 = uVar3 - 1) {
    *pcVar11 = *pcVar9;
    pcVar9 = pcVar9 + 1;
    pcVar11 = pcVar11 + 1;
  }
  uVar3 = 0xffffffff;
  pcVar9 = &DAT_005e03f8;
  do {
    pcVar11 = pcVar9;
    if (uVar3 == 0) break;
    uVar3 = uVar3 - 1;
    pcVar11 = pcVar9 + 1;
    cVar1 = *pcVar9;
    pcVar9 = pcVar11;
  } while (cVar1 != '\0');
  uVar3 = ~uVar3;
  iVar8 = 0;
  iVar5 = -1;
  pcVar9 = (char *)&DAT_00d3a4c0;
  do {
    pcVar10 = pcVar9;
    if (iVar5 == 0) break;
    iVar5 = iVar5 + -1;
    pcVar10 = pcVar9 + 1;
    cVar1 = *pcVar9;
    pcVar9 = pcVar10;
  } while (cVar1 != '\0');
  pcVar9 = pcVar11 + -uVar3;
  pcVar11 = pcVar10 + -1;
  for (uVar4 = uVar3 >> 2; uVar4 != 0; uVar4 = uVar4 - 1) {
    *(undefined4 *)pcVar11 = *(undefined4 *)pcVar9;
    pcVar9 = pcVar9 + 4;
    pcVar11 = pcVar11 + 4;
  }
  iVar5 = 0;
  for (uVar3 = uVar3 & 3; uVar3 != 0; uVar3 = uVar3 - 1) {
    *pcVar11 = *pcVar9;
    pcVar9 = pcVar9 + 1;
    pcVar11 = pcVar11 + 1;
  }
  do {
    uVar3 = 1 << ((byte)iVar5 & 0x1f);
    if ((*(uint *)(&DAT_00799ad8 + iVar2) & uVar3) != 0) {
      uVar3 = 0xffffffff;
      pcVar9 = &DAT_005e03f8;
      do {
        pcVar11 = pcVar9;
        if (uVar3 == 0) break;
        uVar3 = uVar3 - 1;
        pcVar11 = pcVar9 + 1;
        cVar1 = *pcVar9;
        pcVar9 = pcVar11;
      } while (cVar1 != '\0');
      uVar3 = ~uVar3;
      iVar6 = -1;
      pcVar9 = (char *)&DAT_00d3a4c0;
      do {
        pcVar10 = pcVar9;
        if (iVar6 == 0) break;
        iVar6 = iVar6 + -1;
        pcVar10 = pcVar9 + 1;
        cVar1 = *pcVar9;
        pcVar9 = pcVar10;
      } while (cVar1 != '\0');
      pcVar9 = pcVar11 + -uVar3;
      pcVar11 = pcVar10 + -1;
      for (uVar4 = uVar3 >> 2; uVar4 != 0; uVar4 = uVar4 - 1) {
        *(undefined4 *)pcVar11 = *(undefined4 *)pcVar9;
        pcVar9 = pcVar9 + 4;
        pcVar11 = pcVar11 + 4;
      }
      iVar8 = iVar8 + 1;
      for (uVar3 = uVar3 & 3; uVar3 != 0; uVar3 = uVar3 - 1) {
        *pcVar11 = *pcVar9;
        pcVar9 = pcVar9 + 1;
        pcVar11 = pcVar11 + 1;
      }
      FUN_00561a36(local_8,&DAT_005e03f0,iVar8);
      uVar3 = 0xffffffff;
      pcVar9 = local_8;
      do {
        pcVar11 = pcVar9;
        if (uVar3 == 0) break;
        uVar3 = uVar3 - 1;
        pcVar11 = pcVar9 + 1;
        cVar1 = *pcVar9;
        pcVar9 = pcVar11;
      } while (cVar1 != '\0');
      uVar3 = ~uVar3;
      iVar6 = -1;
      pcVar9 = (char *)&DAT_00d3a4c0;
      do {
        pcVar10 = pcVar9;
        if (iVar6 == 0) break;
        iVar6 = iVar6 + -1;
        pcVar10 = pcVar9 + 1;
        cVar1 = *pcVar9;
        pcVar9 = pcVar10;
      } while (cVar1 != '\0');
      pcVar9 = pcVar11 + -uVar3;
      pcVar11 = pcVar10 + -1;
      for (uVar4 = uVar3 >> 2; uVar4 != 0; uVar4 = uVar4 - 1) {
        *(undefined4 *)pcVar11 = *(undefined4 *)pcVar9;
        pcVar9 = pcVar9 + 4;
        pcVar11 = pcVar11 + 4;
      }
      for (uVar3 = uVar3 & 3; uVar3 != 0; uVar3 = uVar3 - 1) {
        *pcVar11 = *pcVar9;
        pcVar9 = pcVar9 + 1;
        pcVar11 = pcVar11 + 1;
      }
      if (iVar5 + 0x8f == 0) {
        pcVar9 = &DAT_005e03f8;
      }
      else {
        FUN_004cc110(0x42,iVar5 + 0x8f);
        pcVar9 = DAT_00e20768;
      }
      uVar4 = 0xffffffff;
      uVar3 = 0;
      do {
        pcVar11 = pcVar9;
        if (uVar4 == 0) break;
        uVar4 = uVar4 - 1;
        pcVar11 = pcVar9 + 1;
        cVar1 = *pcVar9;
        pcVar9 = pcVar11;
      } while (cVar1 != '\0');
      uVar4 = ~uVar4;
      iVar6 = -1;
      pcVar9 = (char *)&DAT_00d3a4c0;
      do {
        pcVar10 = pcVar9;
        if (iVar6 == 0) break;
        iVar6 = iVar6 + -1;
        pcVar10 = pcVar9 + 1;
        cVar1 = *pcVar9;
        pcVar9 = pcVar10;
      } while (cVar1 != '\0');
      pcVar9 = pcVar11 + -uVar4;
      pcVar11 = pcVar10 + -1;
      for (uVar7 = uVar4 >> 2; uVar7 != 0; uVar7 = uVar7 - 1) {
        *(undefined4 *)pcVar11 = *(undefined4 *)pcVar9;
        pcVar9 = pcVar9 + 4;
        pcVar11 = pcVar11 + 4;
      }
      for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
        *pcVar11 = *pcVar9;
        pcVar9 = pcVar9 + 1;
        pcVar11 = pcVar11 + 1;
      }
    }
    iVar5 = iVar5 + 1;
    if (0xd < iVar5) {
      return CONCAT31((int3)(uVar3 >> 8),1);
    }
  } while( true );
}
