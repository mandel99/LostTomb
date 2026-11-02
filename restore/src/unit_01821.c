#include "restore_types.h"
#include "mission_loaders.h"
#include "chunk_io.h"
#include "mission_routes.h"
#include "mission_schema.h"
#define DAT_005c1140 (*(undefined4 *)0x005c1140u)
#define DAT_005c1350 (*(undefined4 *)0x005c1350u)
#define DAT_005e09a0 (*(undefined4 *)0x005e09a0u)
#define DAT_005e45b0 (*(undefined4 *)0x005e45b0u)
#define DAT_005e46a0 (*(undefined4 *)0x005e46a0u)
#define DAT_007027b4 (*(undefined1 *)0x007027b4u)
#define DAT_00776fb4 (*(undefined4 *)0x00776fb4u)
#define DAT_007770e4 (*(int32_t *)0x007770e4u)
#define DAT_007770f1 (*(undefined1 *)0x007770f1u)
#define DAT_00784b94 (*(uint16_t *)0x00784b94u)
#define DAT_00784cbc (*(byte *)0x00784cbcu)
#define DAT_00784ce0 (*(undefined4 *)0x00784ce0u)
#define DAT_00784d3a (*(undefined *)0x00784d3au)
#define DAT_0078d798 (*(int32_t *)0x0078d798u)
#define DAT_0078d79c (*(int32_t *)0x0078d79cu)
#define DAT_0078d7a0 (*(int32_t *)0x0078d7a0u)
#define DAT_0078d7a4 (*(int32_t *)0x0078d7a4u)
#define DAT_0078d7a8 (*(int32_t *)0x0078d7a8u)
#define DAT_0078d7ac (*(int32_t *)0x0078d7acu)
#define DAT_0078d7b0 (*(int32_t *)0x0078d7b0u)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define DAT_00978d9c (*(undefined4 *)0x00978d9cu)
#define DAT_00d38374 (*(int32_t *)0x00d38374u)
#define DAT_00e39080 (*(int32_t *)0x00e39080u)
#define DAT_00e945c0 (*(int32_t *)0x00e945c0u)
#define DAT_00e945c4 (*(int32_t *)0x00e945c4u)
#define DAT_00ea6018 (*(undefined4 *)0x00ea6018u)
#define DAT_00eaac28 (*(int32_t *)0x00eaac28u)
#define DAT_00eaaf5c (*(undefined1 *)0x00eaaf5cu)
#define _DAT_00d38380 (*(undefined4 *)0x00d38380u)
extern void FUN_004206a0();
extern void FUN_004209e0();
extern void FUN_00420d10();
extern void FUN_00421010();
extern void FUN_004212f0();
extern void FUN_004bd5d0();
extern void FUN_004d0a40();
extern void FUN_004d17e0();
extern void FUN_004d1840();
extern void FUN_004d1860();
extern void FUN_004d1880();
extern void FUN_004d18b0();
extern void FUN_004d18f0();
extern void FUN_004d19e0();
extern void FUN_004d1a70();
extern void FUN_004d1ad0();
extern void FUN_004d2fa0();
extern void FUN_004d3200();
extern void FUN_004d32a0();
extern void FUN_004d32d0();
extern void FUN_004d32f0();
extern void FUN_004d3c20();
extern void FUN_004d3c40();
extern void FUN_004d3cc0();
extern void FUN_004d3d00();
extern void FUN_004d3d80();
extern void FUN_004d3da0();
extern void FUN_004d3f40();
extern void FUN_004d3f60();
extern void FUN_004d4040();
extern void FUN_004d4080();
extern void FUN_004d40a0();
extern void FUN_004d40c0();
extern void FUN_004d4180();
extern void FUN_004de140();
extern void FUN_00561a2b();

void FUN_004d41e0(int param_1)

{
  short sVar1;
  bool bVar2;
  bool bVar3;
  bool bVar4;
  bool bVar5;
  bool bVar6;
  char cVar7;
  ushort *puVar8;
  int iVar9;
  int iVar10;
  short *psVar11;
  undefined4 *puVar12;
  int *piVar13;
  undefined4 *puVar14;
  undefined4 local_5e8 [60];
  undefined4 local_4f8 [78];
  undefined4 local_3c0 [108];
  undefined4 local_210 [132];
  
  DAT_00e945c0 = 0xa0;
  puVar12 = &DAT_005e45b0;
  puVar14 = local_5e8;
  for (iVar9 = 0x3c; iVar9 != 0; iVar9 = iVar9 + -1) {
    *puVar14 = *puVar12;
    puVar12 = puVar12 + 1;
    puVar14 = puVar14 + 1;
  }
  DAT_00e945c4 = 0;
  puVar12 = &DAT_005e46a0;
  puVar14 = local_3c0;
  for (iVar9 = 0x6c; iVar9 != 0; iVar9 = iVar9 + -1) {
    *puVar14 = *puVar12;
    puVar12 = puVar12 + 1;
    puVar14 = puVar14 + 1;
  }
  puVar12 = &DAT_005c1140;
  puVar14 = local_210;
  for (iVar9 = 0x84; iVar9 != 0; iVar9 = iVar9 + -1) {
    *puVar14 = *puVar12;
    puVar12 = puVar12 + 1;
    puVar14 = puVar14 + 1;
  }
  puVar12 = &DAT_005c1350;
  puVar14 = local_4f8;
  for (iVar9 = 0x4e; iVar9 != 0; iVar9 = iVar9 + -1) {
    *puVar14 = *puVar12;
    puVar12 = puVar12 + 1;
    puVar14 = puVar14 + 1;
  }
  piVar13 = &DAT_00776fb4;
  do {
    if (*piVar13 != 0) {
      FUN_00561a2b(*piVar13);
      *piVar13 = 0;
      piVar13[0x4d] = 0;
    }
    piVar13 = piVar13 + 0x51;
  } while ((int)piVar13 < 0x77aefc);
  iVar9 = 0;
  do {
    if ((DAT_00e945c0 < 0x9a) && (iVar9 == 0xaf)) {
      FUN_004d3c40();
      break;
    }
    cVar7 = FUN_004d0e10(iVar9,(RestoreChunkDescriptor *)&DAT_005e09a0,param_1);
    if (cVar7 == '\0') {
      return;
    }
    iVar9 = iVar9 + 1;
  } while (iVar9 < 0xb5);
  DAT_007770f1 = 0;
  DAT_007770e4 = 0;
  piVar13 = &DAT_00776fb4;
  do {
    if (((((*piVar13 == 0) && (piVar13[0x4d] == 0)) && (*(byte *)((int)piVar13 + 0x13d) < 0x33)) &&
        ((*(byte *)(piVar13 + 0x4f) < 3 && (*(byte *)(piVar13 + -1) != 0)))) &&
       (*(byte *)(piVar13 + -1) < 0x33)) {
      FUN_004d4720((RestoreMissionRoute *)(piVar13-1));
    }
    else {
      piVar13[0x4c] = 0;
      *piVar13 = 0;
      *(undefined1 *)((int)piVar13 + 0x13d) = 0;
      piVar13[0x4d] = 0;
      *(undefined1 *)(piVar13 + -1) = 5;
      *(undefined1 *)((int)piVar13 + 0x13e) = 0;
      *(undefined2 *)(piVar13 + 0x4e) = 0xffff;
      *(undefined2 *)((int)piVar13 + 0x13a) = 0xffff;
      *(undefined1 *)(piVar13 + 0x4f) = 1;
    }
    iVar9 = DAT_00e945c0;
    piVar13 = piVar13 + 0x51;
  } while ((int)piVar13 < 0x77aefc);
  if (DAT_00e945c0 < 0x75) {
    puVar8 = (ushort *)&DAT_00784ce0;
    do {
      *puVar8 = (ushort)((int)puVar8 < 0x784d44);
      puVar8 = puVar8 + 1;
    } while ((int)puVar8 < 0x784da8);
  }
  if (iVar9 < 0x97) {
    DAT_00784b94 = 0;
  }
  if (iVar9 < 0x76) {
    FUN_004d17e0();
    iVar9 = DAT_00e945c0;
  }
  if (iVar9 < 0x78) {
    FUN_004d1840();
    iVar9 = DAT_00e945c0;
  }
  if (iVar9 < 0x79) {
    FUN_004d1860();
    iVar9 = DAT_00e945c0;
  }
  if (iVar9 < 0x7a) {
    FUN_004d1880();
    iVar9 = DAT_00e945c0;
  }
  if (iVar9 < 0x7b) {
    FUN_004d18b0();
    iVar9 = DAT_00e945c0;
  }
  if (iVar9 < 0x7c) {
    FUN_004d18f0();
    FUN_004d1ad0();
    FUN_004d2fa0();
    FUN_004d3200();
    iVar9 = DAT_00e945c0;
  }
  if (iVar9 < 0x7d) {
    FUN_004bd330(RESTORE_LOAD_SETTINGS);
    iVar9 = DAT_00e945c0;
  }
  if (iVar9 < 0x84) {
    FUN_004bd5d0();
  }
  FUN_004d0a40(1);
  if (DAT_00e945c0 < 0x7f) {
    FUN_004d32d0();
  }
  iVar9 = DAT_00e945c0;
  if (DAT_00e945c0 < 0x80) {
    puVar12 = &DAT_00978d9c;
    for (iVar10 = 0xcb10; iVar10 != 0; iVar10 = iVar10 + -1) {
      *puVar12 = 0;
      puVar12 = puVar12 + 1;
    }
  }
  if (iVar9 < 0x81) {
    FUN_004d32a0();
  }
  FUN_004d32f0();
  iVar9 = DAT_00e945c0;
  if (DAT_00e945c0 < 0x85) {
    DAT_007027b4 = 0;
  }
  if (DAT_00e945c0 < 0x86) {
    *(undefined4 *)(uintptr_t)(0xea6018u + DAT_00eaac28 * 0x49d8u) = DAT_00e39080;
  }
  if (iVar9 < 0x88) {
    FUN_004d3c20();
    iVar9 = DAT_00e945c0;
  }
  if (iVar9 < 0x8b) {
    FUN_004d3cc0();
    iVar9 = DAT_00e945c0;
  }
  if (iVar9 < 0x8f) {
    FUN_004de140();
    iVar9 = DAT_00e945c0;
  }
  if (iVar9 < 0x90) {
    FUN_004d3d80();
    iVar9 = DAT_00e945c0;
  }
  if (iVar9 < 0x92) {
    FUN_004d3da0();
    FUN_004d3f40();
    iVar9 = DAT_00e945c0;
  }
  if (iVar9 < 0x93) {
    _DAT_00d38380 = DAT_00d38374;
  }
  if (iVar9 < 0x94) {
    FUN_004d3d00();
    iVar9 = DAT_00e945c0;
  }
  if (iVar9 < 0x95) {
    FUN_004d3f60();
    iVar9 = DAT_00e945c0;
  }
  if (iVar9 < 0x96) {
    FUN_004d4040();
    iVar9 = DAT_00e945c0;
  }
  if (iVar9 < 0x98) {
    FUN_004d40c0();
    iVar9 = DAT_00e945c0;
  }
  if (iVar9 < 0x99) {
    FUN_004d19e0();
    iVar9 = DAT_00e945c0;
  }
  if (iVar9 < 0x9b) {
    FUN_004d4080();
    FUN_004d40a0();
    puVar12 = (undefined4 *)&DAT_00784d3a;
    for (iVar9 = 0x1b; iVar9 != 0; iVar9 = iVar9 + -1) {
      *puVar12 = 0;
      puVar12 = puVar12 + 1;
    }
    *(undefined2 *)puVar12 = 0;
    iVar9 = DAT_00e945c0;
  }
  if (iVar9 < 0x9c) {
    FUN_004d1a70();
    iVar9 = DAT_00e945c0;
  }
  if (iVar9 < 0x9d) {
    DAT_0078d7b0 = 0;
    DAT_0078d7a8 = 0;
    DAT_0078d7a4 = 0;
    RESTORE_SLICE1(DAT_00784cbc,0) = 1;
  }
  if (iVar9 < 0x9e) {
    DAT_0078d7ac = 0;
  }
  if (iVar9 < 0x9f) {
    RESTORE_SLICE1(DAT_00784cbc,1) = 0;
  }
  if (iVar9 < 0xa0) {
    DAT_0078d79c = 0;
    FUN_004d4180();
    DAT_0078d798 = 10;
    DAT_0078d7a0 = 0;
  }
  bVar6 = false;
  bVar3 = false;
  bVar4 = false;
  bVar5 = false;
  bVar2 = false;
  psVar11 = (short *)&DAT_00799af0;
  do {
    if ((char)psVar11[-8] == '\x01') {
      sVar1 = *psVar11;
      if (sVar1 == 0xe3) {
        bVar3 = true;
      }
      else if (sVar1 == 0xe4) {
        bVar6 = true;
      }
      else if (sVar1 == 0xe1) {
        bVar4 = true;
      }
      else if (sVar1 == 0xe6) {
        bVar5 = true;
      }
      else if (((sVar1 == 0xe5) || (sVar1 == 0xea)) || ((sVar1 == 0xeb || (sVar1 == 0xec)))) {
        bVar2 = true;
      }
    }
    psVar11 = psVar11 + 0x84;
  } while ((int)psVar11 < 0x89b6e8);
  if (!bVar3) {
    FUN_00420d10();
  }
  if (!bVar6) {
    FUN_00421010();
  }
  if (!bVar4) {
    FUN_004206a0();
  }
  if (!bVar5) {
    FUN_004209e0();
  }
  if (!bVar2) {
    FUN_004212f0();
  }
  iVar9 = 0;
  do {
    *(undefined4 *)((int)&DAT_005e45b0 + iVar9) = *(undefined4 *)((int)local_5e8 + iVar9);
    iVar9 = iVar9 + 0x18;
  } while (iVar9 < 0xf0);
  iVar9 = 0;
  do {
    *(undefined4 *)((int)&DAT_005e46a0 + iVar9) = *(undefined4 *)((int)local_3c0 + iVar9);
    iVar9 = iVar9 + 0x18;
  } while (iVar9 < 0x1b0);
  iVar9 = 0;
  do {
    *(undefined4 *)((int)&DAT_005c1140 + iVar9) = *(undefined4 *)((int)local_210 + iVar9);
    iVar9 = iVar9 + 0x18;
  } while (iVar9 < 0x210);
  iVar9 = 0;
  do {
    *(undefined4 *)((int)&DAT_005c1350 + iVar9) = *(undefined4 *)((int)local_4f8 + iVar9);
    iVar9 = iVar9 + 0x18;
  } while (iVar9 < 0x138);
  DAT_00eaaf5c = 0 < DAT_0078d7a4;
  restore_mission_schema_recover_loaded();
  return;
}
