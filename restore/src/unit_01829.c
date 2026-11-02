#include "restore_types.h"
#include "world_route_geometry.h"
#define DAT_005e17c4 (*(undefined4 *)0x005e17c4u)
#define DAT_005e17cc (*(undefined4 *)0x005e17ccu)
#define DAT_005e17d8 (*(undefined *)0x005e17d8u)
#define DAT_005e17e4 (*(undefined *)0x005e17e4u)
#define DAT_00776fb4 (*(undefined4 *)0x00776fb4u)
#define DAT_007770e4 (*(undefined *)0x007770e4u)
#define DAT_007770f1 (*(undefined1 *)0x007770f1u)
#define DAT_00784bbc (*(undefined *)0x00784bbcu)
#define DAT_00784ca4 (*(undefined *)0x00784ca4u)
#define DAT_00784ca8 (*(undefined *)0x00784ca8u)
#define DAT_00784cac (*(undefined *)0x00784cacu)
#define DAT_00784cb1 (*(undefined1 *)0x00784cb1u)
#define DAT_00e92e50 (*(undefined4 *)0x00e92e50u)
#define DAT_00e945c0 (*(undefined *)0x00e945c0u)
#define DAT_00e945c4 (*(undefined *)0x00e945c4u)
#define DAT_00f565dc (*(undefined *)0x00f565dcu)
#define DAT_00f569d4 (*(undefined *)0x00f569d4u)
extern int FUN_00445d10();
extern void FUN_004771c0();
extern void FUN_00478420();
extern void FUN_00486e70();
extern void FUN_00489030();
extern void FUN_004ce2d0();
extern undefined4 FUN_0055bc50();
extern void * FUN_00561a1d();
extern void FUN_00561a2b();
extern undefined4 FUN_005647eb();
extern int FUN_005664d5();
extern uint FUN_00568eab();

undefined4 FUN_004d5520(byte *param_1)

{
  char cVar1;
  byte bVar2;
  byte bVar3;
  int iVar4;
  void *pvVar5;
  uint uVar6;
  uint uVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  byte *pbVar11;
  int iVar12;
  char *pcVar13;
  int *piVar14;
  undefined4 *puVar15;
  char *pcVar16;
  undefined4 *puVar17;
  int local_40;
  undefined1 auStack_34 [52];
  
  DAT_00784ca4 = DAT_00f569d4;
  DAT_00784ca8 = DAT_00f569d4 + 2;
  DAT_00784cac = DAT_00f565dc;
  DAT_00784cb1 = 0;
  DAT_00784bbc = 0;
  FUN_004ce2d0();
  uVar6 = 0xffffffff;
  pcVar13 = &DAT_005e17cc;
  do {
    pcVar16 = pcVar13;
    if (uVar6 == 0) break;
    uVar6 = uVar6 - 1;
    pcVar16 = pcVar13 + 1;
    cVar1 = *pcVar13;
    pcVar13 = pcVar16;
  } while (cVar1 != '\0');
  uVar6 = ~uVar6;
  pcVar13 = pcVar16 + -uVar6;
  pcVar16 = (char *)&DAT_005e17c4;
  for (uVar7 = uVar6 >> 2; uVar7 != 0; uVar7 = uVar7 - 1) {
    *(undefined4 *)pcVar16 = *(undefined4 *)pcVar13;
    pcVar13 = pcVar13 + 4;
    pcVar16 = pcVar16 + 4;
  }
  DAT_00e945c0 = 0xa0;
  for (uVar6 = uVar6 & 3; uVar6 != 0; uVar6 = uVar6 - 1) {
    *pcVar16 = *pcVar13;
    pcVar13 = pcVar13 + 1;
    pcVar16 = pcVar16 + 1;
  }
  iVar4 = FUN_00568eab(param_1,0x8301,0x180);
  if (iVar4 == -1) {
    return 1;
  }
  FUN_00478420();
  FUN_00486e70();
  FUN_00489030();
  FUN_004771c0();
  DAT_007770f1 = 0;
  DAT_007770e4 = 0;
  piVar14 = &DAT_00776fb4;
  do {
    if (*piVar14 != 0) {
      FUN_00561a2b(*piVar14);
      *piVar14 = 0;
      piVar14[0x4d] = 0;
    }
    piVar14 = piVar14 + 0x51;
  } while ((int)piVar14 < 0x77aefc);
  puVar15 = &DAT_00e92e50;
  for (iVar8 = 0x5dc; iVar8 != 0; iVar8 = iVar8 + -1) {
    *puVar15 = 0;
    puVar15 = puVar15 + 1;
  }
  puVar15 = &DAT_005e17d8;
  puVar17 = &DAT_00e92e50;
  for (iVar8 = 0x78; iVar8 != 0; iVar8 = iVar8 + -1) {
    *puVar17 = *puVar15;
    puVar15 = puVar15 + 1;
    puVar17 = puVar17 + 1;
  }
  DAT_00e945c4 = 0x18;
  piVar14 = &DAT_005e17e4;
  do {
    if (piVar14[-3] == 0) {
      FUN_005664d5(iVar4,piVar14[-2],*piVar14 * piVar14[-1]);
    }
    else {
      FUN_0055bc50(iVar4,piVar14[-2],*piVar14 * piVar14[-1]);
    }
    piVar14 = piVar14 + 5;
  } while ((int)piVar14 < 0x5e19c4);
  param_1 = &DAT_007770f1;
  do {
    if (((((*(int *)(param_1 + -0x13d) == 0) && (*(int *)(param_1 + -9) == 0)) &&
         (bVar2 = *param_1, bVar2 < 0x33)) &&
        ((param_1[-1] < 3 && (bVar3 = param_1[-0x141], bVar3 != 0)))) && (bVar3 < 0x33)) {
      iVar12 = 1;
      iVar8 = 0;
      if (1 < bVar2) {
        pbVar11 = param_1 + -0x139;
        do {
          local_40 = (int)*(short *)pbVar11 - (int)*(short *)(pbVar11 + 6);
          iVar9 = (int)*(short *)(pbVar11 + 6) - (int)*(short *)pbVar11;
          if (local_40 <= iVar9) {
            local_40 = iVar9;
          }
          iVar9 = (int)*(short *)(pbVar11 + 2) - (int)*(short *)(pbVar11 + 8);
          iVar10 = (int)*(short *)(pbVar11 + 8) - (int)*(short *)(pbVar11 + 2);
          if (iVar9 <= iVar10) {
            iVar9 = iVar10;
          }
          if (iVar9 < local_40) {
            iVar8 = iVar8 + 1 + local_40;
            auStack_34[iVar12] = 1;
          }
          else {
            iVar8 = iVar8 + 1 + iVar9;
            auStack_34[iVar12] = 0;
          }
          iVar12 = iVar12 + 1;
          pbVar11 = pbVar11 + 6;
        } while (iVar12 < (int)(uint)bVar2);
      }
      if ((*(int *)(param_1 + -9) < iVar8) &&
         (iVar12 = iVar8 + 0x32, *(int *)(param_1 + -9) < iVar12)) {
        if (*(int *)(param_1 + -0x13d) != 0) {
          FUN_00561a2b(*(int *)(param_1 + -0x13d));
          param_1[-0xffffffff0000013d] = 0;
          param_1[-0xffffffff0000013c] = 0;
          param_1[-0xffffffff0000013b] = 0;
          param_1[-0xffffffff0000013a] = 0;
        }
        pvVar5 = FUN_00561a1d(iVar12 * 6);
        *(void **)(param_1 + -0x13d) = pvVar5;
        *(int *)(param_1 + -9) = iVar12;
      }
      *(int *)(param_1 + -0xd) = iVar8;
      iVar12 = 0;
      iVar8 = 1;
      if (1 < *param_1) {
        do {
          if (*(int *)(param_1 + -0xd) < iVar12) goto LAB_004d57c3;
          iVar9 = FUN_00445d10(iVar8 + -1,iVar8,iVar12);
          iVar12 = iVar12 + iVar9;
          iVar8 = iVar8 + 1;
        } while (iVar8 < (int)(uint)*param_1);
      }
      *(int *)(param_1 + -0xd) = iVar12;
      FUN_00407f90((RestoreWorldRoute *)(param_1-0x141));
    }
    else {
      param_1[-0xffffffff0000000d] = 0;
      param_1[-0xffffffff0000000c] = 0;
      param_1[-0xffffffff0000000b] = 0;
      param_1[-0xffffffff0000000a] = 0;
      param_1[-0xffffffff0000013d] = 0;
      param_1[-0xffffffff0000013c] = 0;
      param_1[-0xffffffff0000013b] = 0;
      param_1[-0xffffffff0000013a] = 0;
      *param_1 = 0;
      param_1[-0xffffffff00000009] = 0;
      param_1[-0xffffffff00000008] = 0;
      param_1[-0xffffffff00000007] = 0;
      param_1[-0xffffffff00000006] = 0;
      param_1[-0x141] = 5;
      param_1[1] = 0;
      param_1[-0xffffffff00000005] = 0xff;
      param_1[-0xffffffff00000004] = 0xff;
      param_1[-0xffffffff00000003] = 0xff;
      param_1[-0xffffffff00000002] = 0xff;
      param_1[-1] = 1;
    }
LAB_004d57c3: ;
    param_1 = param_1 + 0x144;
    if (0x77b038 < (int)param_1) {
      FUN_005647eb(iVar4);
      return 0;
    }
  } while( true );
}
