#include "restore_types.h"
#include "mission_loaders.h"
#include "chunk_io.h"
#include "mission_routes.h"
#define DAT_005e17c4 (*(undefined4 *)0x005e17c4u)
#define DAT_005e17cc (*(undefined4 *)0x005e17ccu)
#define DAT_005e17d8 (*(int32_t *)0x005e17d8u)
#define DAT_005e17e0 (*(int32_t *)0x005e17e0u)
#define DAT_005e17e4 (*(int32_t *)0x005e17e4u)
#define DAT_00776fb4 (*(undefined4 *)0x00776fb4u)
#define DAT_007770e4 (*(int32_t *)0x007770e4u)
#define DAT_007770f1 (*(undefined1 *)0x007770f1u)
#define DAT_00784b94 (*(uint16_t *)0x00784b94u)
#define DAT_00784cbc (*(byte *)0x00784cbcu)
#define DAT_00784ce0 (*(undefined4 *)0x00784ce0u)
#define DAT_00784d3a (*(undefined *)0x00784d3au)
#define DAT_0078d798 (*(int32_t *)0x0078d798u)
#define DAT_0078d7a0 (*(int32_t *)0x0078d7a0u)
#define DAT_0078d7a4 (*(int32_t *)0x0078d7a4u)
#define DAT_0078d7a8 (*(int32_t *)0x0078d7a8u)
#define DAT_0078d7ac (*(int32_t *)0x0078d7acu)
#define DAT_0078d7b0 (*(int32_t *)0x0078d7b0u)
#define DAT_00e20764 (*(uint32_t *)0x00e20764u)
#define DAT_00e20768 (*(uint32_t *)0x00e20768u)
#define DAT_00e945c0 (*(int32_t *)0x00e945c0u)
#define DAT_00e945c4 (*(int32_t *)0x00e945c4u)
#define PTR_DAT_005e17dc (*(unsigned char * *)0x005e17dcu)
extern void FUN_004cc110();
extern void FUN_004cc160();
extern void FUN_004d17e0();
extern void FUN_004d19e0();
extern void FUN_004d1a70();
extern void FUN_004d32d0();
extern void FUN_004d3d80();
extern void FUN_004d4080();
extern void FUN_004d40a0();
extern void FUN_004d40c0();
extern int FUN_00538cd0();
extern undefined4 FUN_0055bcf0();
extern void FUN_00561a2b();
extern int FUN_00562370();
extern int FUN_005660ed();

void FUN_004d4ae0(int param_1)

{
  char cVar1;
  int iVar2;
  ushort *puVar3;
  int iVar4;
  int *piVar5;
  undefined4 *puVar6;
  
  if (DAT_005e17d8 == 0) {
    FUN_005660ed(param_1,PTR_DAT_005e17dc,DAT_005e17e4 * DAT_005e17e0);
  }
  else {
    FUN_0055bcf0(param_1,PTR_DAT_005e17dc,DAT_005e17e4 * DAT_005e17e0);
  }
  iVar2 = FUN_00562370((char *)&DAT_005e17c4,&DAT_005e17cc);
  if (iVar2 == 0) {
    DAT_00e945c0 = 0xa0;
    DAT_00e945c4 = 0;
    piVar5 = &DAT_00776fb4;
    do {
      if (*piVar5 != 0) {
        FUN_00561a2b(*piVar5);
        *piVar5 = 0;
        piVar5[0x4d] = 0;
      }
      piVar5 = piVar5 + 0x51;
    } while ((int)piVar5 < 0x77aefc);
    iVar2 = 1;
    do {
      cVar1 = FUN_004d0e10(iVar2,(RestoreChunkDescriptor *)&DAT_005e17d8,param_1);
      if (cVar1 == '\0') {
        return;
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 < 0x18);
    DAT_007770f1 = 0;
    DAT_007770e4 = 0;
    piVar5 = &DAT_00776fb4;
    do {
      if ((((*piVar5 == 0) && (piVar5[0x4d] == 0)) && (*(byte *)((int)piVar5 + 0x13d) < 0x33)) &&
         (((*(byte *)(piVar5 + 0x4f) < 3 && (*(byte *)(piVar5 + -1) != 0)) &&
          (*(byte *)(piVar5 + -1) < 0x33)))) {
        FUN_004d4720((RestoreMissionRoute *)(piVar5-1));
      }
      else {
        piVar5[0x4c] = 0;
        *piVar5 = 0;
        *(undefined1 *)((int)piVar5 + 0x13d) = 0;
        piVar5[0x4d] = 0;
        *(undefined1 *)(piVar5 + -1) = 5;
        *(undefined1 *)((int)piVar5 + 0x13e) = 0;
        *(undefined2 *)(piVar5 + 0x4e) = 0xffff;
        *(undefined2 *)((int)piVar5 + 0x13a) = 0xffff;
        *(undefined1 *)(piVar5 + 0x4f) = 1;
      }
      iVar2 = DAT_00e945c0;
      piVar5 = piVar5 + 0x51;
    } while ((int)piVar5 < 0x77aefc);
    if (DAT_00e945c0 < 0x75) {
      puVar3 = (ushort *)&DAT_00784ce0;
      do {
        *puVar3 = (ushort)((int)puVar3 < 0x784d44);
        puVar3 = puVar3 + 1;
      } while ((int)puVar3 < 0x784da8);
    }
    if (iVar2 < 0x97) {
      DAT_00784b94 = 0;
    }
    if (iVar2 < 0x76) {
      FUN_004d17e0();
      iVar2 = DAT_00e945c0;
    }
    if (iVar2 < 0x7d) {
      FUN_004bd330(RESTORE_LOAD_SETTINGS);
      iVar2 = DAT_00e945c0;
    }
    if (iVar2 < 0x7f) {
      FUN_004d32d0();
      iVar2 = DAT_00e945c0;
    }
    if (iVar2 < 0x90) {
      FUN_004d3d80();
      iVar2 = DAT_00e945c0;
    }
    if (iVar2 < 0x98) {
      FUN_004d40c0();
      iVar2 = DAT_00e945c0;
    }
    if (iVar2 < 0x99) {
      FUN_004d19e0();
      iVar2 = DAT_00e945c0;
    }
    if (iVar2 < 0x9b) {
      FUN_004d4080();
      FUN_004d40a0();
      puVar6 = (undefined4 *)&DAT_00784d3a;
      for (iVar4 = 0x1b; iVar2 = DAT_00e945c0, iVar4 != 0; iVar4 = iVar4 + -1) {
        *puVar6 = 0;
        puVar6 = puVar6 + 1;
      }
      *(undefined2 *)puVar6 = 0;
    }
    if (iVar2 < 0x9c) {
      FUN_004d1a70();
      iVar2 = DAT_00e945c0;
    }
    if (iVar2 < 0x9d) {
      DAT_0078d7b0 = 0;
      DAT_0078d7a8 = 0;
      DAT_0078d7a4 = 0;
      RESTORE_SLICE1(DAT_00784cbc,0) = 1;
    }
    if (iVar2 < 0x9e) {
      DAT_0078d7ac = 0;
      RESTORE_SLICE1(DAT_00784cbc,1) = 0;
      DAT_0078d798 = 10;
      DAT_0078d7a0 = 0;
    }
    return;
  }
  FUN_004cc110(5,0x18);
  FUN_004cc160(5,0x19);
  FUN_00538cd0(DAT_00e20764,DAT_00e20768,0);
  return;
}
