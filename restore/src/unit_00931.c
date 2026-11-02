#include "restore_types.h"
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_00ac2d4c (*(undefined4 *)0x00ac2d4cu)
#define DAT_00f564cc (*(undefined *)0x00f564ccu)
#define DAT_00f56584 (*(undefined *)0x00f56584u)
extern void FUN_0047bf50();

undefined4 FUN_004673c0(int param_1)

{
  int iVar1;
  
  iVar1 = param_1 * 0x108;
  if ((&DAT_007999d8)[iVar1] == '\x01') {
    if ('\x1e' < (char)(&DAT_00ac2d4c)[*(int *)(&DAT_007999e4 + iVar1)]) {
      FUN_0047bf50(param_1,(int)*(short *)((int)&DAT_007999dc + iVar1 + 2),
                   (int)*(short *)(&DAT_007999e0 + iVar1),(&DAT_007999db)[iVar1],
                   (&DAT_007999db)[iVar1],DAT_00f56584,8);
      return 1;
    }
    FUN_0047bf50(param_1,(int)*(short *)((int)&DAT_007999dc + iVar1 + 2),
                 (int)*(short *)(&DAT_007999e0 + iVar1),(&DAT_007999db)[iVar1],
                 (&DAT_007999db)[iVar1],DAT_00f564cc,8);
  }
  return 0;
}
