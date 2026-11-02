#include "restore_types.h"
#define DAT_00d3b5c0 (*(int32_t *)0x00d3b5c0u)
#define DAT_00d3b5dc (*(int32_t *)0x00d3b5dcu)
#define DAT_00d3b5e0 (*(int32_t *)0x00d3b5e0u)
#define DAT_00f5683c (*(int32_t *)0x00f5683cu)
extern int FUN_0053e590();

void FUN_00536820(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined4 local_8;
  
  iVar2 = 0;
  iVar3 = 0;
  local_8 = 0;
  if (0 < param_4) {
    do {
      iVar5 = 0;
      iVar4 = 0;
      if (0 < param_3) {
        iVar1 = local_8 * 0x10 + param_2;
        do {
          DAT_00d3b5e0 = iVar5 * 0x10 + param_1;
          if (local_8 == 0) {
            if (iVar5 == 0) {
              iVar2 = 0;
              DAT_00d3b5c0 = DAT_00f5683c;
            }
            else if (iVar5 < param_3 + -1) {
              DAT_00d3b5c0 = DAT_00f5683c + 1 + iVar4;
              iVar4 = iVar4 + 1;
              iVar2 = 0;
            }
            else {
              DAT_00d3b5c0 = DAT_00f5683c + 0xb;
              iVar2 = 0;
            }
          }
          else if (local_8 < param_4 + -1) {
            if (iVar5 == 0) {
              DAT_00d3b5c0 = DAT_00f5683c + 0xc + iVar3;
            }
            else if (iVar5 < param_3 + -1) {
              iVar2 = DAT_00f5683c + iVar4;
              iVar4 = iVar4 + 1;
              DAT_00d3b5c0 = iVar2 + 0xd + iVar3;
            }
            else {
              DAT_00d3b5c0 = DAT_00f5683c + 0x17 + iVar3;
            }
            iVar2 = 0xc;
          }
          else if (iVar5 == 0) {
            DAT_00d3b5c0 = DAT_00f5683c + 0x84;
            iVar2 = 0;
          }
          else if (iVar5 < param_3 + -1) {
            DAT_00d3b5c0 = DAT_00f5683c + 0x85 + iVar4;
            iVar4 = iVar4 + 1;
            iVar2 = 0;
          }
          else {
            DAT_00d3b5c0 = DAT_00f5683c + 0x8f;
            iVar2 = 0;
          }
          DAT_00d3b5dc = iVar1;
          FUN_0053e590(DAT_00d3b5c0,DAT_00d3b5e0,iVar1);
          if (9 < iVar4) {
            iVar4 = 0;
          }
          iVar5 = iVar5 + 1;
        } while (iVar5 < param_3);
      }
      iVar3 = iVar3 + iVar2;
      if (0x77 < iVar3) {
        iVar3 = 0;
      }
      local_8 = local_8 + 1;
    } while (local_8 < param_4);
  }
  return;
}
