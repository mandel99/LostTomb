#include "restore_types.h"
#include "simulation_tick.h"
#define DAT_00605a3c (*(uint32_t *)0x00605a3cu)
#define DAT_00d386d4 (*(uint32_t *)0x00d386d4u)
#define DAT_00d3b1c4 (*(int32_t *)0x00d3b1c4u)
#define DAT_00d3b1c8 (*(undefined4 *)0x00d3b1c8u)
#define DAT_00d3b374 (*(uint32_t *)0x00d3b374u)
#define DAT_00d3b380 (*(uint32_t *)0x00d3b380u)
#define DAT_00d3b384 (*(uint32_t *)0x00d3b384u)
#define DAT_00ea6208 (*(undefined4 *)0x00ea6208u)
#define DAT_00eaac28 (*(uint32_t *)0x00eaac28u)
#define _DAT_00d3b364 (*(undefined4 *)0x00d3b364u)
#define _DAT_00d3b368 (*(undefined4 *)0x00d3b368u)
#define _DAT_00d3b36c (*(undefined4 *)0x00d3b36cu)
#define _DAT_00d3b370 (*(undefined4 *)0x00d3b370u)
#define _DAT_00d3b378 (*(undefined4 *)0x00d3b378u)
extern void FUN_0041ff60();
extern void FUN_00451c50();
extern void FUN_00485dc0();
extern void FUN_00489b70();
extern void FUN_004cdc70();
extern void FUN_004e7e40();
extern void FUN_004ea2c0();

void FUN_004d7090(void)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  
  (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
  DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
  if (99 < DAT_00d3b1c4) {
    DAT_00d3b1c4 = 0;
  }
  iVar5 = 0x1f;
  do {
    uVar1 = DAT_00d3b384 >> 4;
    uVar3 = DAT_00d3b384 & 1;
    uVar4 = DAT_00d3b380 >> 4;
    uVar2 = DAT_00d3b380 & 1;
    DAT_00d3b384 = DAT_00d3b384 >> 1;
    DAT_00d3b380 = DAT_00d3b380 >> 1;
    if ((uVar1 & 1) != uVar3) {
      DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
    }
    if ((uVar4 & 1) != uVar2) {
      DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
    }
    iVar5 = iVar5 + -1;
  } while (iVar5 != 0);
  _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
  DAT_00d3b374 = DAT_00d3b384 & 0x7f;
  _DAT_00d3b378 = DAT_00d3b384 & 7;
  _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
  _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
  _DAT_00d3b36c = DAT_00d3b380 & 7;
  FUN_004cdc70();
  FUN_00485dc0();
  if (DAT_00605a3c == 0) {
    FUN_00451c50();
    FUN_004e7e40();
    FUN_004be770(0x00d05720u);
    if ((DAT_00d386d4 == 3) && (*(int *)(&DAT_00ea6208 + DAT_00eaac28 * 0x49d8) == 2)) {
      *(undefined4 *)(&DAT_00ea6208 + DAT_00eaac28 * 0x49d8) = 0;
    }
  }
  FUN_00489b70();
  FUN_0041ff60();
  FUN_0044bca0((int32_t *)0x00785d70u);
  FUN_004ea2c0();
  return;
}
