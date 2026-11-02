#include "restore_types.h"
#define DAT_0056f7d8 (*(undefined4 *)0x0056f7d8u)
#define DAT_0056f7ec (*(undefined4 *)0x0056f7ecu)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
extern void FUN_004ee5d0();

void FUN_004ee630(int param_1)

{
  byte *pbVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = 0;
  pbVar1 = (byte *)((int)&DAT_00799a2c + param_1 * 0x108 + 1);
  while( true ) {
    while( true ) {
      if (DAT_005e4ef4 == 5) {
        iVar2 = *(int *)(&DAT_0056f7ec + (uint)*(byte *)(&DAT_00799a2c + param_1 * 0x84) * 4);
      }
      else {
        iVar2 = *(int *)(&DAT_0056f7d8 + (uint)*(byte *)(&DAT_00799a2c + param_1 * 0x84) * 4);
      }
      if (iVar2 <= iVar3) {
        FUN_004ee5d0(param_1);
        return;
      }
      if (DAT_005e4ef4 == 5) break;
      if (iVar3 < 4) {
        if (*pbVar1 < 0xb) {
          return;
        }
        iVar3 = iVar3 + 1;
        pbVar1 = pbVar1 + 0x108;
      }
      else {
        if (*pbVar1 < 10) {
          return;
        }
        if (pbVar1[6] < 0xf) {
          return;
        }
        iVar3 = iVar3 + 1;
        pbVar1 = pbVar1 + 0x108;
      }
    }
    if (*pbVar1 < 5) {
      return;
    }
    if (pbVar1[6] < 0xf) break;
    iVar3 = iVar3 + 1;
    pbVar1 = pbVar1 + 0x108;
  }
  return;
}
