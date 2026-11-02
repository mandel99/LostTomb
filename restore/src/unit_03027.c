#include "restore_types.h"
#define DAT_0126648c (*(undefined *)0x0126648cu)
extern uint FUN_00566682();

undefined4 FUN_0056b240(byte *param_1,byte *param_2,int param_3)

{
  byte bVar1;
  byte bVar2;
  undefined2 uVar3;
  uint uVar4;
  
  if (param_3 != 0) {
    if (DAT_0126648c == 0) {
      do {
        bVar1 = *param_1;
        param_1 = param_1 + 1;
        bVar2 = *param_2;
        uVar4 = (uint)CONCAT11(bVar1,bVar2);
        param_2 = param_2 + 1;
        if (bVar1 != bVar2) {
          if ((0x40 < bVar1) && (bVar1 < 0x5b)) {
            uVar4 = (uint)CONCAT11(bVar1 + 0x20,bVar2);
          }
          uVar3 = (undefined2)uVar4;
          bVar1 = (byte)uVar4;
          if ((0x40 < bVar1) && (bVar1 < 0x5b)) {
            uVar3 = (undefined2)CONCAT31((int3)(uVar4 >> 8),bVar1 + 0x20);
          }
          bVar1 = (byte)((ushort)uVar3 >> 8);
          if (bVar1 != (byte)uVar3) {
            if ((byte)uVar3 <= bVar1) {
              return 1;
            }
            return 0xffffffff;
          }
        }
        param_3 = param_3 + -1;
      } while (param_3 != 0);
    }
    else {
      do {
        bVar1 = *param_1;
        param_1 = param_1 + 1;
        bVar2 = *param_2;
        param_2 = param_2 + 1;
        if (bVar1 != bVar2) {
          bVar1 = FUN_00566682(bVar2,bVar1);
          bVar2 = FUN_00566682();
          if (bVar2 != bVar1) {
            if (bVar1 <= bVar2) {
              return 1;
            }
            return 0xffffffff;
          }
        }
        param_3 = param_3 + -1;
      } while (param_3 != 0);
    }
  }
  return 0;
}
