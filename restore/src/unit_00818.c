#include "restore_types.h"
#define DAT_0070cdf8 (*(undefined4 *)0x0070cdf8u)
#define DAT_0070cdfc (*(undefined4 *)0x0070cdfcu)
#define DAT_0070ce00 (*(undefined4 *)0x0070ce00u)
#define DAT_0070ce04 (*(undefined4 *)0x0070ce04u)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799a76 (*(undefined4 *)0x00799a76u)
#define DAT_00799a78 (*(undefined4 *)0x00799a78u)
#define DAT_00799a7a (*(undefined4 *)0x00799a7au)
#define DAT_00799ae5 (*(undefined4 *)0x00799ae5u)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)

void FUN_00455cc0(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  char *pcVar4;
  int iVar5;
  int local_4;
  
  if (0 < DAT_0078d790) {
    pcVar4 = &DAT_00799ae5;
    local_4 = DAT_0078d790;
    do {
      if (((pcVar4[-5] == '\x01') && (*pcVar4 != '\0')) && (*(short *)(pcVar4 + 0x17) != 0)) {
        iVar2 = (int)*(short *)(pcVar4 + 1);
        iVar3 = param_1 * 0x108;
        _DAT_00d3b4f8 = (int)*(short *)(pcVar4 + 3);
        iVar1 = (int)*(short *)((int)&DAT_007999dc + iVar3 + 2);
        iVar5 = (int)*(short *)(&DAT_007999e0 + iVar3);
        if (iVar1 < iVar2) {
          DAT_00d3b4fc = iVar2 - iVar1;
        }
        else if (iVar2 < iVar1) {
          DAT_00d3b4fc = iVar1 - iVar2;
        }
        else {
          DAT_00d3b4fc = 0;
        }
        if (iVar5 < _DAT_00d3b4f8) {
          _DAT_00d3b4f8 = _DAT_00d3b4f8 - iVar5;
        }
        else if (_DAT_00d3b4f8 < iVar5) {
          _DAT_00d3b4f8 = iVar5 - _DAT_00d3b4f8;
        }
        else {
          _DAT_00d3b4f8 = 0;
        }
        iVar1 = _DAT_00d3b4f8;
        if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
          iVar1 = DAT_00d3b4fc;
        }
        if (iVar1 <= param_2) {
          iVar1 = (int)*(short *)(pcVar4 + 0xd);
          if (iVar1 < 0x13) {
            iVar1 = iVar1 + 1;
          }
          iVar1 = iVar1 * 0x58;
          iVar2 = *(int *)(&DAT_0070ce04 + iVar1);
          if (1 < (byte)pcVar4[-2]) {
            iVar2 = iVar2 << 2;
          }
          if (iVar2 != 0) {
            *(undefined2 *)((int)&DAT_00799a7a + iVar3 + 2) = 10;
          }
          iVar2 = *(int *)(&DAT_0070cdf8 + iVar1);
          if (1 < (byte)pcVar4[-2]) {
            iVar2 = iVar2 << 2;
          }
          if (iVar2 != 0) {
            *(undefined2 *)(&DAT_00799a76 + iVar3) = 10;
          }
          iVar2 = *(int *)(&DAT_0070cdfc + iVar1);
          if (1 < (byte)pcVar4[-2]) {
            iVar2 = iVar2 << 2;
          }
          if (iVar2 != 0) {
            *(undefined2 *)(&DAT_00799a7a + param_1 * 0x42) = 10;
          }
          iVar1 = *(int *)(&DAT_0070ce00 + iVar1);
          if (1 < (byte)pcVar4[-2]) {
            iVar1 = iVar1 << 2;
          }
          if (iVar1 != 0) {
            *(undefined2 *)(&DAT_00799a78 + iVar3) = 10;
          }
        }
      }
      pcVar4 = pcVar4 + 0x108;
      local_4 = local_4 + -1;
    } while (local_4 != 0);
  }
  return;
}
