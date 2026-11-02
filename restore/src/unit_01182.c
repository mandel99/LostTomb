#include "restore_types.h"
#define DAT_00705c1c (*(undefined *)0x00705c1cu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799a82 (*(undefined4 *)0x00799a82u)
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcd672 (*(undefined4 *)0x00bcd672u)
#define DAT_00ea5cd0 (*(undefined4 *)0x00ea5cd0u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)

int FUN_004809b0(int param_1)

{
  byte bVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  short *psVar7;
  
  if (0 < DAT_00705c1c) {
    bVar1 = (&DAT_00bcd672)[param_1 * 0x184];
    uVar6 = (uint)bVar1;
    if (bVar1 < 10) {
      psVar7 = (short *)((int)&DAT_00ea5cd0 + (DAT_00eaac28 * 0x24ec + uVar6) * 2);
      uVar5 = uVar6;
      do {
        iVar3 = (int)*psVar7;
        if (0 < iVar3) {
          iVar4 = iVar3 * 0x108;
          cVar2 = *(char *)(&DAT_00799a82 + iVar3 * 0x84);
          if (cVar2 == '\0') {
            _DAT_00d3b4e4 = *(short *)((int)&DAT_007999dc + iVar4 + 2) + 1;
            _DAT_00d3b4e0 = *(short *)(&DAT_007999e0 + iVar4) + -2;
          }
          else {
            if (cVar2 == '\x01') {
              _DAT_00d3b4e4 = *(short *)((int)&DAT_007999dc + iVar4 + 2) + 4;
            }
            else {
              _DAT_00d3b4e4 = (int)*(short *)((int)&DAT_007999dc + iVar4 + 2);
              if (cVar2 == '\x02') {
                _DAT_00d3b4e4 = _DAT_00d3b4e4 + 1;
                _DAT_00d3b4e0 = *(short *)(&DAT_007999e0 + iVar4) + 4;
                goto LAB_00480a78;
              }
              _DAT_00d3b4e4 = _DAT_00d3b4e4 + -2;
            }
            _DAT_00d3b4e0 = *(short *)(&DAT_007999e0 + iVar4) + 1;
          }
LAB_00480a78: ;
          if (*(short *)(&DAT_00b350dc + (DAT_00bca2d8 + _DAT_00d3b4e0 * 0xe4 + _DAT_00d3b4e4) * 2)
              == 0) {
            return iVar3;
          }
        }
        uVar5 = uVar5 + 1;
        psVar7 = psVar7 + 1;
      } while ((int)uVar5 < 10);
    }
    if (bVar1 < 10) {
      psVar7 = (short *)((int)&DAT_00ea5cd0 + (DAT_00eaac28 * 0x24ec + uVar6) * 2);
      do {
        iVar3 = (int)*psVar7;
        if (0 < iVar3) {
          iVar4 = iVar3 * 0x108;
          cVar2 = *(char *)(&DAT_00799a82 + iVar3 * 0x84);
          if (cVar2 == '\0') {
            _DAT_00d3b4e4 = *(short *)((int)&DAT_007999dc + iVar4 + 2) + 1;
            _DAT_00d3b4e0 = *(short *)(&DAT_007999e0 + iVar4) + -3;
          }
          else {
            if (cVar2 == '\x01') {
              _DAT_00d3b4e4 = *(short *)((int)&DAT_007999dc + iVar4 + 2) + 5;
            }
            else {
              _DAT_00d3b4e4 = (int)*(short *)((int)&DAT_007999dc + iVar4 + 2);
              if (cVar2 == '\x02') {
                _DAT_00d3b4e4 = _DAT_00d3b4e4 + 1;
                _DAT_00d3b4e0 = *(short *)(&DAT_007999e0 + iVar4) + 5;
                goto LAB_00480b49;
              }
              _DAT_00d3b4e4 = _DAT_00d3b4e4 + -3;
            }
            _DAT_00d3b4e0 = *(short *)(&DAT_007999e0 + iVar4) + 1;
          }
LAB_00480b49: ;
          if (*(short *)(&DAT_00b350dc + (DAT_00bca2d8 + _DAT_00d3b4e0 * 0xe4 + _DAT_00d3b4e4) * 2)
              == 0) {
            return iVar3;
          }
        }
        uVar6 = uVar6 + 1;
        psVar7 = psVar7 + 1;
      } while ((int)uVar6 < 10);
    }
  }
  return 0;
}
