#include "restore_types.h"
#define DAT_00705c1c (*(undefined *)0x00705c1cu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799a82 (*(undefined4 *)0x00799a82u)
#define DAT_00799a8c (*(undefined4 *)0x00799a8cu)
#define DAT_00bcd672 (*(undefined4 *)0x00bcd672u)
#define DAT_00ea5cd0 (*(undefined4 *)0x00ea5cd0u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)

int FUN_004808a0(int param_1)

{
  char cVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  short *psVar5;
  undefined2 uVar6;
  
  if (DAT_00705c1c < 1) {
    return 0;
  }
  uVar4 = (uint)(byte)(&DAT_00bcd672)[param_1 * 0x184];
  if ((byte)(&DAT_00bcd672)[param_1 * 0x184] < 10) {
    psVar5 = (short *)((int)&DAT_00ea5cd0 + (DAT_00eaac28 * 0x24ec + uVar4) * 2);
    do {
      iVar2 = (int)*psVar5;
      if ((0 < iVar2) &&
         ((*(short *)(&DAT_00799a8c + iVar2 * 0x108) == 0 ||
          (*(short *)(&DAT_00799a8c + iVar2 * 0x108) == param_1)))) break;
      uVar4 = uVar4 + 1;
      psVar5 = psVar5 + 1;
    } while ((int)uVar4 < 10);
    if (0 < iVar2) {
      iVar3 = iVar2 * 0x108;
      cVar1 = *(char *)(&DAT_00799a82 + iVar2 * 0x84);
      uVar6 = (undefined2)param_1;
      if (cVar1 == '\0') {
        _DAT_00d3b4e4 = *(short *)((int)&DAT_007999dc + iVar3 + 2) + 1;
        *(undefined2 *)(&DAT_00799a8c + iVar3) = uVar6;
        _DAT_00d3b4e0 = *(short *)(&DAT_007999e0 + iVar3) + -1;
        return iVar2;
      }
      if (cVar1 == '\x01') {
        _DAT_00d3b4e4 = *(short *)((int)&DAT_007999dc + iVar3 + 2) + 3;
      }
      else {
        _DAT_00d3b4e4 = (int)*(short *)((int)&DAT_007999dc + iVar3 + 2);
        if (cVar1 == '\x02') {
          _DAT_00d3b4e4 = _DAT_00d3b4e4 + 1;
          *(undefined2 *)(&DAT_00799a8c + iVar3) = uVar6;
          _DAT_00d3b4e0 = *(short *)(&DAT_007999e0 + iVar3) + 3;
          return iVar2;
        }
        _DAT_00d3b4e4 = _DAT_00d3b4e4 + -1;
      }
      *(undefined2 *)(&DAT_00799a8c + iVar3) = uVar6;
      _DAT_00d3b4e0 = *(short *)(&DAT_007999e0 + iVar3) + 1;
      return iVar2;
    }
  }
  return 0;
}
