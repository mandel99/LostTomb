#include "restore_types.h"
#define DAT_005f81e4 (*(undefined4 *)0x005f81e4u)
#define DAT_005f81e8 (*(undefined4 *)0x005f81e8u)
#define DAT_005f81ec (*(undefined4 *)0x005f81ecu)
#define DAT_00d3b5c0 (*(undefined *)0x00d3b5c0u)
#define DAT_00d3b5dc (*(undefined *)0x00d3b5dcu)
#define DAT_00d3b5e0 (*(undefined *)0x00d3b5e0u)
#define DAT_00f53f10 (*(undefined4 *)0x00f53f10u)
#define DAT_00f56494 (*(undefined *)0x00f56494u)
extern int FUN_0053e590();
extern int FUN_0053f4f0();

void FUN_00536dc0(int param_1,int param_2,int param_3,int param_4,int param_5,int param_6)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int local_4;
  
  iVar3 = *(int *)(&DAT_00f53f10 + param_1 * 4);
  if (4 < iVar3) {
    if (iVar3 < 0xf) {
      iVar3 = (iVar3 * 3 + -0xf) * 4;
      DAT_00d3b5e0 = ((param_4 + -2) * 0x10) / 2 + *(int *)(&DAT_005f81e8 + iVar3) + param_2;
      DAT_00d3b5dc = ((param_5 + -2) * 0x10) / 2 + *(int *)(&DAT_005f81ec + iVar3) + param_3;
      FUN_0053e590(*(int *)(&DAT_005f81e4 + iVar3) + 0x18 + param_6 * 8 + DAT_00f56494,DAT_00d3b5e0,
                   DAT_00d3b5dc);
      return;
    }
    iVar3 = iVar3 + -0xd;
    if (param_4 < iVar3) {
      local_4 = param_4;
    }
    else {
      param_2 = param_2 + ((param_4 - iVar3) * 0x10) / 2;
      local_4 = iVar3;
    }
    if (iVar3 <= param_5) {
      param_3 = param_3 + ((param_5 - iVar3) * 0x10) / 2;
      param_5 = iVar3;
    }
    iVar3 = 0;
    if (0 < param_5) {
      do {
        iVar4 = 0;
        if (0 < local_4) {
          iVar2 = iVar3 * 0x10 + param_3;
          do {
            DAT_00d3b5e0 = iVar4 * 0x10 + param_2;
            if (iVar3 == 0) {
              if (iVar4 == 0) {
                DAT_00d3b5c0 = param_6 * 9 + 6 + DAT_00f56494;
              }
              else {
                iVar1 = DAT_00f56494 + param_6 * 8;
                if (iVar4 < local_4 + -1) {
                  DAT_00d3b5c0 = param_6 + 7 + iVar1;
                }
                else {
                  DAT_00d3b5c0 = param_6 + 8 + iVar1;
                }
              }
            }
            else if (iVar3 < param_5 + -1) {
              if (iVar4 == 0) {
                DAT_00d3b5c0 = DAT_00f56494 + (param_6 + 1) * 9;
              }
              else {
                iVar1 = DAT_00f56494 + param_6 * 8;
                if (iVar4 < local_4 + -1) {
                  DAT_00d3b5c0 = param_6 + 10 + iVar1;
                }
                else {
                  DAT_00d3b5c0 = param_6 + 0xb + iVar1;
                }
              }
            }
            else if (iVar4 == 0) {
              DAT_00d3b5c0 = param_6 * 9 + 0xc + DAT_00f56494;
            }
            else {
              iVar1 = DAT_00f56494 + param_6 * 8;
              if (iVar4 < local_4 + -1) {
                DAT_00d3b5c0 = param_6 + 0xd + iVar1;
              }
              else {
                DAT_00d3b5c0 = param_6 + 0xe + iVar1;
              }
            }
            DAT_00d3b5dc = iVar2;
            if (param_1 == 8) {
              FUN_0053f4f0(DAT_00d3b5c0,DAT_00d3b5e0,iVar2,0xfffff863);
            }
            else {
              FUN_0053e590(DAT_00d3b5c0,DAT_00d3b5e0,iVar2);
            }
            iVar4 = iVar4 + 1;
          } while (iVar4 < local_4);
        }
        iVar3 = iVar3 + 1;
      } while (iVar3 < param_5);
    }
  }
  return;
}
