#include "restore_types.h"
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern undefined4 FUN_004806c0();

undefined1 FUN_00522d80(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined1 uVar4;
  
  iVar2 = _DAT_00d3b4e4;
  iVar1 = _DAT_00d3b4e0;
  uVar4 = 1;
  iVar3 = (short)(&DAT_00bcd652)[param_1 * 0xc2] * 0x108;
  FUN_004806c0((int)*(short *)((int)&DAT_007999dc + iVar3 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar3),(&DAT_007999db)[iVar3]);
  if ((*(short *)(&DAT_00bcd61c + param_1 * 0x184) != _DAT_00d3b4e4) ||
     (*(short *)(&DAT_00bcd61e + param_1 * 0x184) != _DAT_00d3b4e0)) {
    uVar4 = 0;
  }
  _DAT_00d3b4e4 = iVar2;
  _DAT_00d3b4e0 = iVar1;
  return uVar4;
}
