#include "restore_types.h"
#include "chunk_io.h"
#define DAT_00e20764 (*(uint32_t *)0x00e20764u)
#define DAT_00e20768 (*(uint32_t *)0x00e20768u)
#define DAT_00e92e50 (*(undefined4 *)0x00e92e50u)
#define DAT_00e92e58 (*(undefined4 *)0x00e92e58u)
#define DAT_00e92e5c (*(undefined4 *)0x00e92e5cu)
#define DAT_00e945c0 (*(int32_t *)0x00e945c0u)
#define DAT_00e945c4 (*(int32_t *)0x00e945c4u)
extern void FUN_004cc110();
extern void FUN_004cc160();
extern int FUN_00538cd0();
extern undefined4 FUN_0055bcf0();
extern void * FUN_00561a1d();
extern void FUN_00561a2b();
extern undefined4 * FUN_00562880();
extern int FUN_005660ed();

uint8_t FUN_004d0e10(int param_1,RestoreChunkDescriptor *descriptors,int param_3)

{
  int param_2=(int)(uintptr_t)descriptors;
  int *piVar1;
  int *piVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  bool bVar6;
  undefined4 *puVar7;
  uint uVar8;
  int iVar9;
  uint uVar10;
  undefined4 *puVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  undefined4 *puVar16;
  int local_1c;
  int local_10;
  
  bVar6 = false;
  iVar9 = param_1 * 0x14;
  iVar14 = *(int *)(iVar9 + 0xc + param_2);
  iVar4 = *(int *)(iVar9 + 8 + param_2);
  piVar2 = (int *)(iVar9 + 0xc + param_2);
  if (((DAT_00e945c4 != 0) && (*(undefined4 **)(iVar9 + 4 + param_2) != &DAT_00e92e50)) &&
     ((iVar4 != *(int *)(uintptr_t)(0xe92e58u+iVar9) || (iVar14 != *(int *)(uintptr_t)(0xe92e5cu+iVar9))))) {
    bVar6 = true;
  }
  iVar5 = *(int *)(iVar9 + 0x10 + param_2);
  piVar3 = (int *)(iVar9 + 0x10 + param_2);
  local_1c = 0;
  if (iVar5 == 0) {
LAB_004d0eb4: ;
    if (!bVar6) {
      if (*(int *)(iVar9 + param_2) == 0) {
        FUN_005660ed(param_3,*(undefined4 *)(iVar9 + 4 + param_2),iVar14 * iVar4);
      }
      else {
        FUN_0055bcf0(param_3,*(undefined4 *)(iVar9 + 4 + param_2),iVar14 * iVar4);
      }
      goto LAB_004d12ef;
    }
  }
  else {
    iVar15 = iVar5 + 8;
    iVar12 = *(int *)(iVar5 + 8);
    local_1c = 0;
    while ((iVar12 != 0 && (iVar12 < DAT_00e945c0))) {
      piVar1 = (int *)(iVar15 + 0x10);
      iVar15 = iVar15 + 0x10;
      local_1c = local_1c + 1;
      iVar12 = *piVar1;
    }
    if (*(int *)(iVar5 + 8 + local_1c * 0x10) == 0) goto LAB_004d0eb4;
  }
  iVar5 = *(int *)(uintptr_t)(0xe92e5cu+iVar9);
  iVar15 = *(int *)(uintptr_t)(0xe92e58u+iVar9);
  uVar10 = iVar5 * iVar15;
  iVar12 = (&DAT_00e92e50)[param_1 * 5];
  if (iVar14 * iVar4 < (int)uVar10) {
    puVar7 = FUN_00561a1d(uVar10);
    if (puVar7 == (undefined4 *)0x0) {
      return 0;
    }
    puVar11 = puVar7;
    for (uVar8 = uVar10 >> 2; uVar8 != 0; uVar8 = uVar8 - 1) {
      *puVar11 = 0;
      puVar11 = puVar11 + 1;
    }
    for (uVar8 = uVar10 & 3; uVar8 != 0; uVar8 = uVar8 - 1) {
      *(undefined1 *)puVar11 = 0;
      puVar11 = (undefined4 *)((int)puVar11 + 1);
    }
    if (iVar12 == 0) {
      FUN_005660ed(param_3,puVar7,uVar10);
    }
    else {
      FUN_0055bcf0(param_3,puVar7,uVar10);
    }
    if ((*piVar3 != 0) && (iVar4 < iVar15)) {
      iVar12 = 0;
      if (0 < iVar5) {
        puVar11 = puVar7;
        local_10 = iVar5;
        do {
          restore_chunk_migrate(*(uint32_t *)(uintptr_t)(local_1c * 0x10 + 0xc + *piVar3),puVar11,1);
          puVar11 = (undefined4 *)((int)puVar11 + iVar15);
          local_10 = local_10 + -1;
          iVar12 = iVar5;
        } while (local_10 != 0);
      }
      if (iVar12 < iVar14) {
        puVar11 = (undefined4 *)(iVar12 * iVar15 + (int)puVar7);
        iVar12 = iVar14 - iVar12;
        do {
          restore_chunk_migrate(*(uint32_t *)(uintptr_t)(local_1c * 0x10 + 0xc + *piVar3),puVar11,0);
          puVar11 = (undefined4 *)((int)puVar11 + iVar15);
          iVar12 = iVar12 + -1;
        } while (iVar12 != 0);
      }
    }
    iVar12 = iVar14 + -1;
    if (-1 < iVar12) {
      piVar2 = (int *)(iVar9 + 4 + param_2);
      iVar13 = iVar12 * iVar4;
      puVar11 = (undefined4 *)(iVar12 * iVar15 + (int)puVar7);
      local_10 = iVar14;
      do {
        FUN_00562880(iVar13 + *piVar2,puVar11,iVar15);
        if (iVar15 < iVar4) {
          puVar16 = (undefined4 *)(iVar13 + *piVar2 + iVar15);
          for (uVar10 = (uint)(iVar4 - iVar15) >> 2; uVar10 != 0; uVar10 = uVar10 - 1) {
            *puVar16 = 0;
            puVar16 = puVar16 + 1;
          }
          for (uVar10 = iVar4 - iVar15 & 3; uVar10 != 0; uVar10 = uVar10 - 1) {
            *(undefined1 *)puVar16 = 0;
            puVar16 = (undefined4 *)((int)puVar16 + 1);
          }
        }
        puVar11 = (undefined4 *)((int)puVar11 - iVar15);
        iVar13 = iVar13 - iVar4;
        local_10 = local_10 + -1;
      } while (local_10 != 0);
    }
    if ((*piVar3 != 0) && (iVar15 <= iVar4)) {
      iVar12 = 0;
      if (0 < iVar5) {
        puVar11 = puVar7;
        local_10 = iVar5;
        do {
          restore_chunk_migrate(*(uint32_t *)(uintptr_t)(local_1c * 0x10 + 0xc + *piVar3),puVar11,1);
          puVar11 = (undefined4 *)((int)puVar11 + iVar15);
          local_10 = local_10 + -1;
          iVar12 = iVar5;
        } while (local_10 != 0);
      }
      if (iVar12 < iVar14) {
        puVar11 = (undefined4 *)(iVar12 * iVar15 + (int)puVar7);
        iVar12 = iVar14 - iVar12;
        do {
          restore_chunk_migrate(*(uint32_t *)(uintptr_t)(local_1c * 0x10 + 0xc + *piVar3),puVar11,0);
          puVar11 = (undefined4 *)((int)puVar11 + iVar15);
          iVar12 = iVar12 + -1;
        } while (iVar12 != 0);
      }
    }
    if ((*piVar3 == 0) && (iVar5 < iVar14)) {
      uVar8 = (iVar14 - iVar5) * iVar4;
      puVar11 = (undefined4 *)(iVar5 * iVar4 + *(int *)(iVar9 + 4 + param_2));
      for (uVar10 = uVar8 >> 2; uVar10 != 0; uVar10 = uVar10 - 1) {
        *puVar11 = 0;
        puVar11 = puVar11 + 1;
      }
      for (uVar8 = uVar8 & 3; uVar8 != 0; uVar8 = uVar8 - 1) {
        *(undefined1 *)puVar11 = 0;
        puVar11 = (undefined4 *)((int)puVar11 + 1);
      }
    }
    FUN_00561a2b(puVar7);
  }
  else {
    piVar1 = (int *)(iVar9 + 4 + param_2);
    puVar7 = *(undefined4 **)(iVar9 + 4 + param_2);
    for (uVar8 = uVar10 >> 2; uVar8 != 0; uVar8 = uVar8 - 1) {
      *puVar7 = 0;
      puVar7 = puVar7 + 1;
    }
    for (uVar8 = uVar10 & 3; uVar8 != 0; uVar8 = uVar8 - 1) {
      *(undefined1 *)puVar7 = 0;
      puVar7 = (undefined4 *)((int)puVar7 + 1);
    }
    if (iVar12 == 0) {
      FUN_005660ed(param_3,*piVar1,uVar10);
    }
    else {
      FUN_0055bcf0(param_3,*piVar1,uVar10);
    }
    iVar14 = iVar5 + -1;
    if (-1 < iVar14) {
      uVar10 = iVar4 - iVar15;
      iVar9 = iVar14 * iVar15;
      iVar14 = iVar14 * iVar4;
      param_2 = iVar5;
      do {
        FUN_00562880(*piVar1 + iVar14,iVar9 + *piVar1,iVar15);
        if (0 < (int)uVar10) {
          puVar7 = (undefined4 *)(iVar14 + *piVar1 + iVar15);
          for (uVar8 = uVar10 >> 2; uVar8 != 0; uVar8 = uVar8 - 1) {
            *puVar7 = 0;
            puVar7 = puVar7 + 1;
          }
          for (uVar8 = uVar10 & 3; uVar8 != 0; uVar8 = uVar8 - 1) {
            *(undefined1 *)puVar7 = 0;
            puVar7 = (undefined4 *)((int)puVar7 + 1);
          }
        }
        iVar9 = iVar9 - iVar15;
        iVar14 = iVar14 - iVar4;
        param_2 = param_2 + -1;
      } while (param_2 != 0);
    }
    if (*piVar3 == 0) {
      if (iVar5 < *piVar2) {
        uVar8 = (*piVar2 - iVar5) * iVar4;
        puVar7 = (undefined4 *)(iVar5 * iVar4 + *piVar1);
        for (uVar10 = uVar8 >> 2; uVar10 != 0; uVar10 = uVar10 - 1) {
          *puVar7 = 0;
          puVar7 = puVar7 + 1;
        }
        for (uVar8 = uVar8 & 3; uVar8 != 0; uVar8 = uVar8 - 1) {
          *(undefined1 *)puVar7 = 0;
          puVar7 = (undefined4 *)((int)puVar7 + 1);
        }
      }
    }
    else {
      iVar14 = 0;
      if (0 < iVar5) {
        iVar9 = 0;
        param_3 = iVar5;
        do {
          restore_chunk_migrate(*(uint32_t *)(uintptr_t)(local_1c * 0x10 + 0xc + *piVar3),(void *)(uintptr_t)(iVar9 + *piVar1),1);
          iVar9 = iVar9 + iVar4;
          param_3 = param_3 + -1;
          iVar14 = iVar5;
        } while (param_3 != 0);
      }
      if (iVar14 < *piVar2) {
        iVar9 = iVar14 * iVar4;
        do {
          restore_chunk_migrate(*(uint32_t *)(uintptr_t)(local_1c * 0x10 + 0xc + *piVar3),(void *)(uintptr_t)(iVar9 + *piVar1),0);
          iVar14 = iVar14 + 1;
          iVar9 = iVar9 + iVar4;
        } while (iVar14 < *piVar2);
      }
    }
  }
LAB_004d12ef: ;
  if (DAT_00e945c0 < 0x73) {
    FUN_004cc110(5,0x18);
    FUN_004cc160(5,0x19);
    FUN_00538cd0(DAT_00e20764,DAT_00e20768,0);
    return 0;
  }
  return 1;
}
