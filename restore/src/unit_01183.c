#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799a82 (*(undefined4 *)0x00799a82u)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)

int FUN_00480b80(int param_1)

{
  char cVar1;
  short sVar2;
  int iVar3;
  short *psVar4;
  int iVar5;
  
  iVar3 = 1;
  psVar4 = &DAT_00799af0;
  while ((((char)psVar4[-8] != '\x01' ||
          (((sVar2 = *psVar4, sVar2 != 0x4c && (sVar2 != 0xb5)) && (sVar2 != 0xb6)))) ||
         ((psVar4[0x52] != 0 && (psVar4[0x52] != param_1))))) {
    psVar4 = psVar4 + 0x84;
    iVar3 = iVar3 + 1;
    if (0x89b6e7 < (int)psVar4) {
      return 0;
    }
  }
  if (iVar3 < 1) {
    return 0;
  }
  iVar5 = iVar3 * 0x108;
  cVar1 = *(char *)(&DAT_00799a82 + iVar3 * 0x84);
  if (cVar1 == '\0') {
    _DAT_00d3b4e4 = *(short *)((int)&DAT_007999dc + iVar5 + 2) + 1;
    _DAT_00d3b4e0 = *(short *)(&DAT_007999e0 + iVar5) + -1;
    return iVar3;
  }
  if (cVar1 == '\x01') {
    _DAT_00d3b4e4 = *(short *)((int)&DAT_007999dc + iVar5 + 2) + 2;
  }
  else {
    _DAT_00d3b4e4 = (int)*(short *)((int)&DAT_007999dc + iVar5 + 2);
    if (cVar1 == '\x02') {
      _DAT_00d3b4e4 = _DAT_00d3b4e4 + 1;
      _DAT_00d3b4e0 = *(short *)(&DAT_007999e0 + iVar5) + 2;
      return iVar3;
    }
    _DAT_00d3b4e4 = _DAT_00d3b4e4 + -1;
  }
  _DAT_00d3b4e0 = *(short *)(&DAT_007999e0 + iVar5) + 1;
  return iVar3;
}
