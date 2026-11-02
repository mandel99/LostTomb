#include "restore_types.h"
#define DAT_00d3b1c4 (*(undefined *)0x00d3b1c4u)
#define DAT_00d3b1c8 (*(undefined4 *)0x00d3b1c8u)
#define DAT_00d3b374 (*(undefined *)0x00d3b374u)
#define DAT_00d3b380 (*(undefined *)0x00d3b380u)
#define DAT_00d3b384 (*(undefined *)0x00d3b384u)
#define DAT_00ea1888 (*(undefined4 *)0x00ea1888u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_00d3b364 (*(undefined4 *)0x00d3b364u)
#define _DAT_00d3b368 (*(undefined4 *)0x00d3b368u)
#define _DAT_00d3b36c (*(undefined4 *)0x00d3b36cu)
#define _DAT_00d3b370 (*(undefined4 *)0x00d3b370u)
#define _DAT_00d3b378 (*(undefined4 *)0x00d3b378u)

undefined4 FUN_0042fbb0(void)

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
  _DAT_00d3b378 = DAT_00d3b384 & 7;
  _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
  _DAT_00d3b36c = DAT_00d3b380 & 7;
  DAT_00d3b374 = DAT_00d3b384 & 0x7f;
  _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
  _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
  return CONCAT31((int3)((ulonglong)_DAT_00d3b370 / 100 >> 8),
                  (int)(_DAT_00d3b370 % 100) <
                  0x28 - *(int *)(&DAT_00ea1888 + DAT_00eaac28 * 0x49d8));
}
